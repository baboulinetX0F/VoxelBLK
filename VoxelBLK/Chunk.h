#pragma once

#include "Renderer.h"
#include "Block.h"

static const int CHUNK_SIZE = 16;

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
  Block _blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; 
  Mesh* _mesh;

  void generateMesh();
};