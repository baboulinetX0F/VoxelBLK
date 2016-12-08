#pragma once

#include "Renderer.h"
#include "ChunkManager.h"
#include "UIManager.h"

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
	UIManager* _uiManager;

	// Methods
	void Initialize();
	void gameLoop();
};