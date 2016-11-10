#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void initWindow(const char* title, int width, int height);
	GLFWwindow* getWindow();
	void setWindowSize(int width, int height);
	void setWindowTitle(const char* title);

	void beginFrame();
	void endFrame();

	// Methods related to mesh
	void LoadMesh(Mesh* mesh);
	void UnloadMesh(Mesh* mesh);
	void RenderMesh(Mesh* mesh);

	void initCamera();
	Camera* getCamera();

	void initDefaultShader();

private:
	GLFWwindow* _window;
	int _windowWidth, _windowHeight;
	Camera* _camera;
	glm::mat4 _projection;

	Shader* _defaultShader;

	// for frametime calculation
	double _lastTime;
	int _nbFrames;

	// Methods
	void calculateFrameTime();	
};

