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

void Renderer::createWindow(const char * title, int width, int height)
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
