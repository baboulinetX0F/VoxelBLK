#pragma once

#include "Renderer.h"
#include "Block.h"

static const int CHUNK_SIZE = 16;

class ChunkManager;

enum CHUNK_GEN_MODE {
    GEN_FULL,
    GEN_RANDOM,
    GEN_SPHERE,
    GEN_PERLIN
};

enum CHUNK_STATE
{
	UNLOADED,
	MESH_GENERATED,
	LOADED
};

class Chunk
{
 public:
  Chunk();
  Chunk(glm::vec2 pos);
  ~Chunk();

  bool isLoaded();
  bool isEmpty();
 
  void generateMesh();
  void loadChunk(ChunkManager* manager, Renderer* renderer);
  void generateChunk(CHUNK_GEN_MODE fill, ChunkManager* _manager);
  CHUNK_STATE getState() const;

  unsigned int GetVerticesCount() const;
  GLfloat* GetVerticesData() const;
  
 private:
    Block _blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; 
    glm::vec2 _position = glm::vec2(0, 0);
	std::vector<Vertex> _vertices;
	CHUNK_STATE _state = UNLOADED;
	bool _loaded = false;
	bool _empty = true;

    int _blocksCount = 0;   
	
    bool block_visible(int x, int y, int z);
    BlockFaces getBlocksNearby(int x, int y, int z);
};
