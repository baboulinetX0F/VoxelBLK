#include "Game.h"

#include <iostream>

#include "Input.h"


Game::Game()
{
}

Game::~Game()
{
}

void Game::run()
{
	Initialize();
	gameLoop();
}

void Game::Initialize()
{
	_renderer = new Renderer();
	_uiManager = new UIManager();
	_renderer->initWindow("VoxelBLK", 1280, 720);
	_renderer->initDefaultShader();
	_renderer->initCamera();
	Input::initInput(_renderer);
	_uiManager->Initialize(_renderer);
	_chunkManager = new ChunkManager(_renderer);
	_chunkManager->GenerateChunks(GEN_RANDOM);
	//_renderer->setRenderMode(RenderMode::RENDER_WIREFRAME);
}

void Game::gameLoop()
{
	/* Test Mesh View Frustum
	Mesh* frustum = new Mesh();
	GLfloat farDist = 50.0f;
	GLfloat nearDist = 1.0f;
	GLfloat ratio = static_cast<GLfloat>(1280 / 720);
	GLfloat hfar = 2 * tan(45.0f / 2) * farDist;
	GLfloat wfar = hfar * (ratio);

	GLfloat Hnear = 2 * tan(45.0f / 2) * nearDist;
	GLfloat Wnear = Hnear * ratio;
	glm::vec3 p = _renderer->getCamera()->_pos;
	glm::vec3 d = _renderer->getCamera()->_front;
	glm::vec3 up = _renderer->getCamera()->_up;
	glm::vec3 right = glm::cross(up, d);

	glm::vec3 fc = p + d * farDist;
	glm::vec3 ftl = fc + (up * (hfar / 2)) - (right * (wfar / 2));
	glm::vec3 ftr = fc + (up * (hfar / 2)) + (right * (wfar / 2));
	glm::vec3 fbl = fc - (up * (hfar / 2)) - (right * (wfar / 2));
	glm::vec3 fbr = fc - (up * (hfar / 2)) + (right * (wfar / 2));

	glm::vec3 nc = p + d * nearDist;

	glm::vec3 ntl = nc + (up * (Hnear / 2)) - (right * (Wnear / 2));
	glm::vec3 ntr = nc + (up * (Hnear / 2)) + (right * (Wnear / 2));
	glm::vec3 nbl = nc - (up * (Hnear / 2)) - (right * (Wnear / 2));
	glm::vec3 nbr = nc - (up * (Hnear / 2)) + (right * (Wnear / 2));

	glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	frustum->addVertex(ftl,color);
	frustum->addVertex(ftr, color));
	frustum->addVertex(fbl, color));
	frustum->addVertex(fbl, color));
	frustum->addVertex(fbr, color));
	frustum->addVertex(ftr,color);

	frustum->addVertex(ntl, color));
	frustum->addVertex(ntr, color));
	frustum->addVertex(nbl, color));
	frustum->addVertex(nbl, color));
	frustum->addVertex(nbr, color));
	frustum->addVertex(ntr, color));

	frustum->addVertex(ntl, color));
	frustum->addVertex(ftl, color));
	frustum->addVertex(ftr, color));
	frustum->addVertex(ftr, color));
	frustum->addVertex(ntr, color));
	frustum->addVertex(ntl, color));

	frustum->addVertex(nbl, color));
	frustum->addVertex(fbl, color));
	frustum->addVertex(fbr, color));
	frustum->addVertex(fbr, color));
	frustum->addVertex(nbr, color));
	frustum->addVertex(nbl, color));

	_renderer->LoadMesh(frustum);
	*/		
	
	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();
		_uiManager->NewFrame();
		_chunkManager->Update(_renderer);
		_chunkManager->Render(_renderer);		
		_uiManager->Render();
		_renderer->endFrame();
	}	
}
