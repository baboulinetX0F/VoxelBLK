#pragma once

#include "Chunk.h"
#include "Renderer.h"

#include <noise\noise.h>

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
	//glm::vec3 getChunkPos(Chunk* chunk);

private:
	Chunk* _chunks[5][5];
	noise::module::Perlin _noise;

	bool chunk_visible(int x, int y);
};