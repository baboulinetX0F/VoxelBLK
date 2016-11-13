#pragma once

#include "Chunk.h"
#include "Renderer.h"


class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void init();

	void DEBUG_fillChunks(DEBUG_CHUNK d);

	void Update(Renderer* renderer);
	void Render(Renderer* renderer);

private:
	Chunk* _chunks[5][5];	// TODO : Replace with dynamic array (double pointer)

	bool chunk_visible(int x, int y);
};

