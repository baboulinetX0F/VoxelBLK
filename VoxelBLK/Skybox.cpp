#include "Skybox.h"


// TODO : Edit default constructor to have in param custom textures / shader
CSkybox::CSkybox()
{
	_texture = new CubemapTexture("textures/skybox/top.jpg", "textures/skybox/bottom.jpg",
		"textures/skybox/left.jpg", "textures/skybox/right.jpg", "textures/skybox/front.jpg",
		"textures/skybox/back.jpg");	
	_shader = new Shader("shaders/skybox.vert", "shaders/skybox.frag");
}


CSkybox::~CSkybox()
{
}

void CSkybox::Load()
{
	_texture->Load();		
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(defaultskyboxVertices), defaultskyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CSkybox::Draw(glm::mat4 view, glm::mat4 projection)
{	
	glDepthFunc(GL_LEQUAL);
	_shader->Use();	
	_texture->Bind();
	glBindVertexArray(_VAO);
	glUniformMatrix4fv(glGetUniformLocation(_shader->_program, "view"), 1,
		GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(_shader->_program, "projection"), 1,
		GL_FALSE, glm::value_ptr(projection));
	glUniform1i(glGetUniformLocation(_shader->_program, "skybox"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
