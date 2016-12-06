#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

enum RenderMode
{
	RENDER_WIREFRAME,
	RENDER_FILL
};


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
	void RenderMesh(Mesh* mesh, glm::mat4 model, Shader* shader);
	void RenderMesh(Mesh* mesh);	
	void RenderMesh(Mesh* mesh, glm::mat4 model);

	void setRenderMode(RenderMode mode);
	const RenderMode getRenderMode();

	void initCamera();
	Camera* getCamera();

	void initDefaultShader();

	const int getVerticesRendered();

private:
	GLFWwindow* _window;
	int _windowWidth, _windowHeight;
	Camera* _camera;
	glm::mat4 _projection;

	Shader* _defaultShader;
	GLuint _occlusionQuery;

	// debug attributes (frametime, drawcalls etc...)
	double _lastTime;
	int _nbFrames;
	double _frameTime;
	int _dVerticesRendered;
	RenderMode _dRenderMode = RenderMode::RENDER_FILL;

	// debug parameter
	bool _pRenderOcclusionPrimitive = false;
	bool _pEnableOcclusionQueryCulling = false;
	
	// Methods
	void printDebugInfos();
	void calculateFrameTime();

	bool occlusionTest(Mesh* mesh);
};

