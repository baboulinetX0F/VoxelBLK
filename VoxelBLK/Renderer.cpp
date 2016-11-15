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
}

void Renderer::initWindow(const char * title, int width, int height)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
}

GLFWwindow * Renderer::getWindow()
{
	return _window;
}

void Renderer::setWindowSize(int width, int height)
{
  glfwSetWindowSize(_window,width,height);
  glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);
}

void Renderer::setWindowTitle(const char* title)
{
  glfwSetWindowTitle(_window,title);
}

void Renderer::beginFrame()
{	
	calculateFrameTime();
	glfwPollEvents();
	glClearColor(0.258f, 0.523f, 0.95f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame()
{
	glfwSwapBuffers(_window);
}

void Renderer::LoadMesh(Mesh * mesh)
{
	GLfloat* data = mesh->verticesToArray();
	glBindVertexArray(mesh->getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBO());
	glBufferData(GL_ARRAY_BUFFER, mesh->getVertices().size() *(VERTEX_COMPONENT_COUNT * sizeof(GLfloat)), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_COMPONENT_COUNT * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_COMPONENT_COUNT * sizeof(GLfloat), (GLvoid*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::UnloadMesh(Mesh * mesh)
{
	// TODO : Implement UnloadMesh
}

void Renderer::RenderMesh(Mesh * mesh, glm::mat4 model, Shader * shader)
{
	shader->Use();
	glUniformMatrix4fv(glGetUniformLocation(_defaultShader->_program, "view"), 1, GL_FALSE, glm::value_ptr(_camera->getViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(_defaultShader->_program, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
	glUniformMatrix4fv(glGetUniformLocation(_defaultShader->_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(mesh->getVAO());
	glColorMask(false, false, false, false);
	glDepthMask(GL_FALSE);
	if (occlusionTest(mesh))
	{
		glColorMask(true, true, true, true);
		glDepthMask(GL_TRUE);
		glDrawArrays(GL_TRIANGLES, 0, mesh->getVertices().size());
		_debugTmp = mesh->getVertices().size();
		glBindVertexArray(0);
	}
	glColorMask(true, true, true, true);
	glDepthMask(GL_TRUE);
}

void Renderer::RenderMesh(Mesh * mesh)
{
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	RenderMesh(mesh, model, _defaultShader);
}

void Renderer::RenderMesh(Mesh * mesh, glm::mat4 model)
{	
	RenderMesh(mesh, model, _defaultShader);
}

void Renderer::setRenderMode(RenderMode mode)
{
	switch (mode)
	{
	case RENDER_WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case RENDER_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Renderer::initCamera()
{
	_camera = new Camera();
	_projection = glm::perspective(45.0f, (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.1f, 50.0f);
}

Camera * Renderer::getCamera()
{
	return _camera;
}

void Renderer::initDefaultShader()
{
	_defaultShader = new Shader("shaders/default_camera.vert", "shaders/default.frag");
}

void Renderer::printDebugInfos()
{
	printf("%f ms/frame | %f fps | %d vertices rendered\n", _frameTime, 1000 / _frameTime, _debugTmp);
}

void Renderer::calculateFrameTime()
{
	double currentTime = glfwGetTime();
	_nbFrames++;	
	if (currentTime - _lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
										 // printf and reset timer
		_frameTime = 1000.0 / double(_nbFrames);
		this->printDebugInfos();		
		_nbFrames = 0;
		_lastTime += 1.0;
	}
}

bool Renderer::occlusionTest(Mesh * mesh)
{
	
	// Begin occlusion query
	glBeginQuery(GL_SAMPLES_PASSED, _occlusionQuery);
	// Every pixel that passes the depth test now gets added to the result
	glDrawArrays(GL_TRIANGLES, 0, mesh->getVertices().size());
	glEndQuery(GL_SAMPLES_PASSED);
	// Now get tthe number of pixels passed
	int iSamplesPassed = 0;
	glGetQueryObjectiv(_occlusionQuery, GL_QUERY_RESULT, &iSamplesPassed);
	if (iSamplesPassed > 10000)
		return true;
	else
		return false;	
}
