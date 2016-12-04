#include "Input.h"

namespace Input {

	void initInput(Renderer * renderer)
	{
		glfwSetKeyCallback(renderer->getWindow(), key_callback);
		glfwSetCursorPosCallback(renderer->getWindow(), mouse_callback);
		_mainCamera = renderer->getCamera();
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (key == GLFW_KEY_W)
			_mainCamera->processKeyboard(CameraDirection::FORWARD);
		if (key == GLFW_KEY_S)
			_mainCamera->processKeyboard(CameraDirection::BACKWARD);
		if (key == GLFW_KEY_D)
			_mainCamera->processKeyboard(CameraDirection::RIGHT);
		if (key == GLFW_KEY_A)
			_mainCamera->processKeyboard(CameraDirection::LEFT);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{		
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		GLfloat x_offset = xpos - lastX;
		GLfloat y_offset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		_mainCamera->processMouse(x_offset, y_offset);
	}
}