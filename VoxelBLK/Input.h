#pragma once

#include "Renderer.h"

namespace Input
{
	static Camera* _mainCamera = nullptr;
	static int lastX, lastY;
	static bool firstMouse;

	void initInput(Renderer* renderer);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
}
