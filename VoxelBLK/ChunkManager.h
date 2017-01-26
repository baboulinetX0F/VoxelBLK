#pragma once

#include <thread>
#include <future>
#include <chrono>
#include <map>

#include "Chunk.h"
#include "Renderer.h"
#include "SimplexNoise.h"


const int CHUNKS_NUMBER_X = 5;
const int CHUNKS_NUMBER_Y = 5;

class ChunkManager
{
public:
	ChunkManager(Renderer* renderer);
	~ChunkManager();	

	void GenerateChunks(CHUNK_GEN_MODE d);

	void Update(Renderer* renderer);
	void Render(Renderer* renderer);

	int getNoiseValue(float x, float z);
	ManagedVBO* getVBO();

private:	
	std::vector<Chunk*> _chunks;
	SimplexNoise _simplex;
	ManagedVBO* _chunksVBO;
	GLuint _chunksVAO;
	
	std::future<void> _calculationThread;
	std::map<Chunk*, unsigned int> _chunksLoaded;
	
	int _chunkCount = 0;
	
	void Initialize();
	void InitializeBuffers(Renderer* renderer);
	bool IsChunkVisible(int x, int y);

	void UnloadChunk(Chunk* chunk);

	void UpdateMeshes();
	void UpdateVBO(Renderer* renderer);
};