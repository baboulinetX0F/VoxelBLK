#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	// Attributes
	GLFWwindow* _window;

	// Methods
	int init();
	void gameLoop();
};