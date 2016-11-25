#pragma once

#include "Chunk.h"
#include "Renderer.h"

#include <noise\noise.h>

const int CHUNKS_NUMBER_X = 1;
const int CHUNKS_NUMBER_Y = 1;

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void init();

	void DEBUG_fillChunks(CHUNK_GEN_MODE d);

	void Update(Renderer* renderer);
	void Render(Renderer* renderer);

	int getNoiseValue(double x, double z);

private:
	Chunk* _chunks[CHUNKS_NUMBER_X][CHUNKS_NUMBER_Y];
	noise::module::Perlin _noise;

	bool chunk_visible(int x, int y);
};