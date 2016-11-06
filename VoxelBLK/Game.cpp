#include "Game.h"

#include <iostream>

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
	_renderer->initWindow("VoxelBLK", 800, 600);
}

void Game::gameLoop()
{
	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();
		_renderer->endFrame();		
	}
}
