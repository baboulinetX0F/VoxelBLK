#pragma once

#include "Chunk.h"
#include "Renderer.h"
#include "SimplexNoise.h"

const int CHUNKS_NUMBER_X = 5;
const int CHUNKS_NUMBER_Y = 5;

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();	

	void generateChunks(CHUNK_GEN_MODE d);

	void Update(Renderer* renderer);
	void Render(Renderer* renderer);

	int getNoiseValue(float x, float z);

private:
	Chunk* _chunks[CHUNKS_NUMBER_X][CHUNKS_NUMBER_Y];
	SimplexNoise _simplex;

	int _chunkCount = 0;

	bool chunk_visible(int x, int y);

	void init();	
};