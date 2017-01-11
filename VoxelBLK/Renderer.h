#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
#include "Skybox.h"

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
		
	GLuint createVAO();
	GLuint createVBO();
	GLuint createVBOAllocated(GLuint VAO, GLuint dataSize);
	unsigned int LoadToManagedVBO(ManagedVBO* vbo, GLfloat* data, VertexAttrib* attrib, unsigned int size);

	// A rename
	void Render(ManagedVBO* vbo, unsigned int vtxcount);

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
	GLfloat GetRenderingDistance();

 private:
	GLFWwindow* _window;
	int _windowWidth, _windowHeight;
	Camera* _camera;
	glm::mat4 _projection;

	Shader* _defaultShader;
	CSkybox* _skybox;
	GLuint _occlusionQuery;
	GLuint _textureAtlas;
		
	// Debug Informations
	double _lastTime;
	int _nbFrames;
	double _dFrameTime;
	int _dVerticesRendered;
		
	RenderMode _dRenderMode = RenderMode::RENDER_FILL;
	GLfloat _pRenderingDistance = 100.0f;
	bool _pRenderOcclusionPrimitive = false;
	bool _pEnableOcclusionQueryCulling = false;
	bool _pDrawSkybox = true;
	
	void printDebugInfos();
	void calculateFrameTime();

	void loadTextureAtlas();
	
	bool occlusionTest(Mesh* mesh);
};
