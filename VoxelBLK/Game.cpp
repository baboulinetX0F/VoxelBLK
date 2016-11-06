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
	if(init() == 0)	
		gameLoop();
}

int Game::init()
{
	_renderer = new Renderer();
	_renderer->createWindow("VoxelBLK", 800, 600);
}

void Game::gameLoop()
{
	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();
		_renderer->endFrame();		
	}
}
