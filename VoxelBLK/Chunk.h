#pragma once

#include "Renderer.h"
#include "Block.h"

static const int CHUNK_SIZE = 4;
static const int WORLD_HEIGHT = 256;

enum DEBUG_CHUNK
{
	FILL_ALL,
	FILL_RANDOM
};

class Chunk
{
 public:
  Chunk();
  ~Chunk();

  void loadChunk(Renderer* renderer);
  void unloadChunk(Renderer* renderer);
  void renderChunk(Renderer* renderer);

  void DEBUG_fillChunk(DEBUG_CHUNK fill);
  
 private:
  Block _blocks[CHUNK_SIZE][WORLD_HEIGHT][CHUNK_SIZE]; 
  Mesh* _mesh;
    

private:
	void generateMesh();
	bool block_visible(int x, int y, int z);
	int blocksGenerated;
};