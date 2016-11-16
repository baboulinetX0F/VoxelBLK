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
	_renderer->initWindow("VoxelBLK", 1280, 720);
	_renderer->initDefaultShader();
	_renderer->initCamera();
	Input::initInput(_renderer);
	_chunkManager = new ChunkManager();
	_chunkManager->DEBUG_fillChunks(FILL_ALL);
	//_renderer->setRenderMode(RENDER_WIREFRAME);	
}

void Game::gameLoop()
{	

	//Chunk* chunk = new Chunk();
	//chunk->DEBUG_fillChunk(FILL_RANDOM);
	//chunk->loadChunk(_renderer);

	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();

		//chunk->renderChunk(_renderer);
		_chunkManager->Update(_renderer);
		_chunkManager->Render(_renderer);

		_renderer->endFrame();
	}
}
