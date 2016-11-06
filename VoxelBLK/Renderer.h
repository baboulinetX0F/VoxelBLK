#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void createWindow(const char* title, int width, int height);
	GLFWwindow* getWindow();

	void beginFrame();
	void endFrame();

private:
	GLFWwindow* _window;
};

