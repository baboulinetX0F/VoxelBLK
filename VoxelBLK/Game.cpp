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
	_renderer->initWindow("VoxelBLK", 800, 600);
	_renderer->initDefaultShader();
	_renderer->initCamera();
	Input::initInput(_renderer);
}

void Game::gameLoop()
{
	//Shader shad = Shader("default.vert", "default.frag");

	Mesh* mesh = new Mesh();
	mesh->addVertex(glm::vec3(-0.5f, -0.5f, 1.0f));
	mesh->addVertex(glm::vec3(-0.5f, 0.5f, 1.0f));
	mesh->addVertex(glm::vec3(0.5f, 0.5f, 1.0f));
	_renderer->LoadMesh(mesh);

	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();

		//shad.Use();
		_renderer->RenderMesh(mesh);

		_renderer->endFrame();		
	}
}
