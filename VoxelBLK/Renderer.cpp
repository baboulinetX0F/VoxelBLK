#include "Renderer.h"

#include <iostream>

#include <gtc\type_ptr.hpp>


Renderer::Renderer()
{
	if (!glfwInit()) {
		std::cerr << "ERROR : Cannot init glfw" << std::endl;
	}	
}

Renderer::~Renderer()
{
	if (_window != nullptr)
		glfwDestroyWindow(_window);
	glDeleteQueries(1, &_occlusionQuery);
}

void Renderer::initWindow(const char * title, int width, int height)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (_window == nullptr)
	{
		std::cerr << "ERROR : Cannot create window" << std::endl;
		glfwTerminate();		
	}

	glfwMakeContextCurrent(_window);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "ERROR : Cannot init glew" << std::endl;
		glfwTerminate();
	}

	glewExperimental = GL_TRUE;

	glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);	
	int vp_width, vp_height;
	glfwGetFramebufferSize(_window, &vp_width, &vp_height);
	glViewport(0, 0, vp_width, vp_height);

	glEnable(GL_DEPTH_TEST);
	glGenQueries(1, &_occlusionQuery);	

	_skybox = new CSkybox();
	_skybox->Load();

	glGenTextures(1, &_textureAtlas);
	loadTextureAtlas();
}

GLFWwindow * Renderer::getWindow()
{
	return _window;
}

void Renderer::setWindowSize(int width, int height)
{
  glfwSetWindowSize(_window, width, height);
  glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);
}

void Renderer::setWindowTitle(const char* title)
{
  glfwSetWindowTitle(_window, title);
}

void Renderer::beginFrame()
{
	glfwPollEvents();
	_dVerticesRendered = 0;
	glClearColor(0.258f, 0.523f, 0.95f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame()
{	
	glfwSwapBuffers(_window);
}

GLuint Renderer::createVAO()
{
	GLuint newVAO;
	glGenVertexArrays(1, &newVAO);
	return newVAO;
}

GLuint Renderer::createVBO()
{
	GLuint newVBO;
	glGenBuffers(1, &newVBO);
	return newVBO;
}

GLuint Renderer::createVBOAllocated(GLuint VAO, GLuint dataSize)
{
	GLuint newVBO;
	glGenBuffers(1, &newVBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newVBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return newVBO;
}
unsigned int Renderer::LoadToManagedVBO(ManagedVBO * vbo, GLfloat * data, VertexAttrib* attrib, unsigned int size)
{
	return vbo->LoadData(data, attrib, size);
}

void Renderer::Render(ManagedVBO* vbo, unsigned int vtxcount)
{
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	_defaultShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(_defaultShader->_program, "view"), 1,
		GL_FALSE, glm::value_ptr(_camera->getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(_defaultShader->_program, "projection"), 1,
		GL_FALSE, glm::value_ptr(_projection));
	glUniformMatrix4fv(glGetUniformLocation(_defaultShader->_program, "model"), 1,
		GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vbo->GetVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _textureAtlas);

	for (std::map<int, unsigned int>::iterator ite = vbo->_blocks.begin();
		ite != vbo->_blocks.end(); ite++)
	{
		glDrawArrays(GL_TRIANGLES,
			(ite->first * vbo->GetBlockSize()) / (VERTEX_COMPONENT_COUNT * sizeof(float)),
			ite->second / (VERTEX_COMPONENT_COUNT * sizeof(float)));
		_dVerticesRendered += ite->second / (VERTEX_COMPONENT_COUNT * sizeof(float));
	}

	glBindVertexArray(0);
}

void Renderer::RenderSkybox()
{
	if (_pDrawSkybox)
		_skybox->Draw(glm::mat4(glm::mat3(_camera->getViewMatrix())), _projection);
}

void Renderer::setRenderMode(RenderMode mode)
{
	_dRenderMode = mode;
	switch (mode)
	{
	case RENDER_WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case RENDER_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

RenderMode Renderer::getRenderMode() const
{
	return _dRenderMode;
}

void Renderer::initCamera()
{
	_camera = new Camera();
	_projection = glm::perspective(45.0f, (GLfloat)_windowWidth / (GLfloat)_windowHeight,
		0.1f, _pRenderingDistance);
}

Camera * Renderer::getCamera() const
{
	return _camera;
}

void Renderer::initDefaultShader()
{
	_defaultShader = new Shader("shaders/default_textured.vert", "shaders/default_textured.frag");
}

int Renderer::getVerticesRendered() const
{
	return _dVerticesRendered;
}

void Renderer::printDebugInfos()
{
	printf("%f ms/frame | %f fps | %d vertices rendered\n", _dFrameTime, 1000 / _dFrameTime, 
		_dVerticesRendered);
}

void Renderer::calculateFrameTime()
{
	double currentTime = glfwGetTime();
	_nbFrames++;

	// If last prinf() was more than 1 sec ago printf and reset timer
	if (currentTime - _lastTime >= 1.0) {
		_dFrameTime = 1000.0 / static_cast<double>(_nbFrames);
		this->printDebugInfos();		
		_nbFrames = 0;
		_lastTime += 1.0;
	}
}

// TODO : Move into a new class
void Renderer::loadTextureAtlas()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, _textureAtlas);

	//Create storage for the texture
	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		3,                    //No mipmaps as textures are 1x1
		GL_RGBA8,              //Internal format
		1024, 1024,         //width,height
		3                  //Number of layers
	);

	const char* files[] =
	{
		"textures/placeholder_1024.png",
		"textures/grass.jpg",
		"textures/dirt.jpg"
	};

	unsigned char* image;
	int width, height;

	for (int i = 0; i < 3; i++)
	{
		image = SOIL_load_image(files[i], &width, &height, 0, SOIL_LOAD_RGBA);
		//Specify i-essim image
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,                     //Mipmap number
			0, 0, i,               //xoffset, yoffset, zoffset
			1024, 1024, 1,               //width, height, depth
			GL_RGBA,                //format
			GL_UNSIGNED_BYTE,      //type
			image);                //pointer to data
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
}

GLfloat Renderer::GetRenderingDistance() const
{
	return _pRenderingDistance;
}