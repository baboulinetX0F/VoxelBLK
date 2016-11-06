#pragma once

#include "Renderer.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	// Attributes
	Renderer* _renderer;

	// Methods
	void init();
	void gameLoop();
};