#pragma once

#include "Renderer.h"
#include "ChunkManager.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	// Attributes
	Renderer* _renderer;
	ChunkManager* _chunkManager;

	// Methods
	void init();
	void gameLoop();
};