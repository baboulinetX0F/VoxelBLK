#include "Game.h"

#include <iostream>

#include "Input.h"
#include "Chunk.h"

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
}

void Game::gameLoop()
{
	Chunk* chunk = new Chunk();
	chunk->DEBUG_fillChunk(FILL_ALL);
	chunk->loadChunk(_renderer);

	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();

		chunk->renderChunk(_renderer);

		_renderer->endFrame();
	}
}
