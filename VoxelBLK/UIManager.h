#pragma once

#include "Renderer.h"

class UIManager
{
public:
	UIManager();
	~UIManager();

	void Initialize(Renderer* renderer);

	void NewFrame();
	void Render();

private:
	Renderer* _renderer;

};

