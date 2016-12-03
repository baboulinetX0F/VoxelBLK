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
	init();
	gameLoop();
}

void Game::init()
{
	_renderer = new Renderer();
	_uiManager = new UIManager();
	_renderer->initWindow("VoxelBLK", 1280, 720);
	_renderer->initDefaultShader();
	_renderer->initCamera();
	Input::initInput(_renderer);
	_uiManager->init(_renderer);
	//_chunkManager = new ChunkManager();
	//_chunkManager->DEBUG_fillChunks(GEN_SPHERE);
	//_renderer->setRenderMode(RenderMode::RENDER_WIREFRAME);
}

void Game::gameLoop()
{
	Chunk* chunk = new Chunk();
	chunk->generateChunk(GEN_RANDOM, nullptr);
	chunk->loadChunk(_renderer);
	
	//std::cout << _chunkManager->getNoiseValue(1, 1);

	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();
		_uiManager->NewFrame();

		chunk->renderChunk(_renderer);
		//_chunkManager->Update(_renderer);
		//_chunkManager->Render(_renderer);
		_uiManager->Render();		

		_renderer->endFrame();
	}
	
}
