#pragma once

#include <noise\noise.h>

#include "Renderer.h"
#include "Block.h"


static const int CHUNK_SIZE = 16;
static const int WORLD_HEIGHT = 16;

class ChunkManager;

enum CHUNK_GEN_MODE
{
	GEN_FULL,
	GEN_RANDOM,
	GEN_SPHERE,
	GEN_PERLIN
};

class Chunk
{
 public:
  Chunk();
  ~Chunk();

  bool isLoaded();

  void loadChunk(Renderer* renderer);
  void unloadChunk(Renderer* renderer);
  void renderChunk(Renderer* renderer);
  void renderChunk(Renderer * renderer, glm::mat4 model);
  
  void generateChunk(CHUNK_GEN_MODE fill, ChunkManager* _manager);
  
 private:
	Block _blocks[CHUNK_SIZE][WORLD_HEIGHT][CHUNK_SIZE]; 
	Mesh* _mesh;
	Mesh* _occlusionMesh;

	int _blocksCount = 0;
	bool _loaded = false;

	void generateMesh();
	void experimental_genMesh();
	bool block_visible(int x, int y, int z);

	BlockFaces getBlocksNearby(int x, int y, int z);
};