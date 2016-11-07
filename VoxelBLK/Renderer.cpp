#include "Renderer.h"

#include <iostream>


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

	int vp_width, vp_height;
	glfwGetFramebufferSize(_window, &vp_width, &vp_height);
	glViewport(0, 0, vp_width, vp_height);
}

GLFWwindow * Renderer::getWindow()
{
	return _window;
}

void Renderer::beginFrame()
{
	glfwPollEvents();
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::UnloadMesh(Mesh * mesh)
{
}

void Renderer::RenderMesh(Mesh * mesh)
{
	glBindVertexArray(mesh->getVAO());	
	glDrawArrays(GL_TRIANGLES, 0, mesh->getVertices().size());	
	glBindVertexArray(0);
}
