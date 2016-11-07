#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Mesh.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void initWindow(const char* title, int width, int height);
	GLFWwindow* getWindow();

	void beginFrame();
	void endFrame();

	void LoadMesh(Mesh* mesh);
	void UnloadMesh(Mesh* mesh);
	void RenderMesh(Mesh* mesh);

private:
	GLFWwindow* _window;

	// for frametime calculation
	double _lastTime;
	int _nbFrames;

	// Methods
	void calculateFrameTime();
};

