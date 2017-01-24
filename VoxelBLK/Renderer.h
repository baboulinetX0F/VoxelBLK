#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <mutex>

#include "Shader.h"
#include "Vertex.h"
#include "Camera.h"
#include "Skybox.h"

enum RenderMode
{
	RENDER_WIREFRAME,
	RENDER_FILL
};

struct UploadJob
{
	ManagedVBO target;
	GLfloat* data;
	GLintptr offset;
	GLsizeiptr size;
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

	void Render(ManagedVBO* vbo, unsigned int vtxcount);
	void RenderSkybox();	

	void setRenderMode(RenderMode mode);
	RenderMode getRenderMode() const;

	void initCamera();
	Camera* getCamera() const;

	void initDefaultShader();

	int getVerticesRendered() const;
	GLfloat GetRenderingDistance() const;

	GLFWwindow* _loadingContext;

 private:
	GLFWwindow* _window;
	int _windowWidth, _windowHeight;
	Camera* _camera;
	glm::mat4 _projection;	

	Shader* _defaultShader;
	CSkybox* _skybox;
	GLuint _occlusionQuery;
	GLuint _textureAtlas;

	std::stack<UploadJob> _uploadQueue;
		
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
};
