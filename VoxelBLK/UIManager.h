#pragma once

#include "Renderer.h"

class UIManager
{
public:
	UIManager();
	~UIManager();

	void init(Renderer* renderer);

	void NewFrame();
	void Render();

private:
	Renderer* _renderer;

};

