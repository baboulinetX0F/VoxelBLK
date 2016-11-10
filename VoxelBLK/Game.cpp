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

	Mesh* _mesh = new Mesh();
	
	int x = 0, y = 0, z = 0;

	// Back face
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));

	//Front face
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));

	// Left Face
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));

	// Right face
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));

	//Bottom face
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));

	// Top face
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));
	_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));

	_renderer->LoadMesh(_mesh);

	while (!glfwWindowShouldClose(_renderer->getWindow()))
	{
		_renderer->beginFrame();

		//shad.Use();
		_renderer->RenderMesh(_mesh);

		_renderer->endFrame();		
	}
}
