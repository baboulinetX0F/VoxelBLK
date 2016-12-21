#include "ChunkManager.h"


ChunkManager::ChunkManager(Renderer* renderer)
{
	InitializeBuffers(renderer);
	Initialize();
}


ChunkManager::~ChunkManager()
{
}

void ChunkManager::Initialize()
{
	for (int x = 0; x < CHUNKS_NUMBER_X; x++)
	{		
		for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
		{
			_chunks[x][y] = new Chunk(glm::vec2(x,y));
			_chunkCount++;
		}
	}	
}

void ChunkManager::InitializeBuffers(Renderer* renderer)
{
	_chunksVAO = renderer->createVAO();	
	_chunksVBO = new ManagedVBO(_chunksVAO, (CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE)
		* (36 * VERTEX_COMPONENT_COUNT * sizeof(float)), CHUNKS_NUMBER_X * CHUNKS_NUMBER_Y);
}

void ChunkManager::GenerateChunks(CHUNK_GEN_MODE d)
{
	for (int x = 0; x < CHUNKS_NUMBER_X; x++)
	{
		for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
		{
			_chunks[x][y]->generateChunk(d, this);
		}
	}
}

void ChunkManager::Update(Renderer* renderer)
{
	for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
	{
		for (int x = 0; x < CHUNKS_NUMBER_X; x++)  
		{
			if (IsChunkVisible(x,y) && !_chunks[x][y]->isLoaded() && !_chunks[x][y]->isEmpty())
			{
				//_chunks[x][y]->loadChunk(renderer);
				_chunks[x][y]->exp_loadChunk(this, renderer);
				std::cout << "Loading Chunk...\n";
			}			
			else if (!IsChunkVisible(x, y))
			{
				_chunks[x][y]->unloadChunk(renderer);
			}		
		}
	}
}

void ChunkManager::Render(Renderer * renderer)
{
	/*
	for (int x = 0; x < CHUNKS_NUMBER_X; x++)
	{
		for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
		{
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, glm::vec3(x*CHUNK_SIZE, 0.0f, y*CHUNK_SIZE));
			_chunks[x][y]->renderChunk(renderer, model);
		}
	}*/

	unsigned int vtxCount = 0;
	for (int x = 0; x < CHUNKS_NUMBER_X; x++)
	{
		for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
		{
			if (_chunks[x][y]->isLoaded())
			{
				vtxCount += _chunks[x][y]->GetVerticesCount();
			}
		}
	}
	
	renderer->Render(_chunksVBO, vtxCount);	
}

int ChunkManager::getNoiseValue(float x, float z)
{	
	return static_cast<int>(_simplex.noise(x,z) * 16);
}

ManagedVBO * ChunkManager::getVBO()
{
	return _chunksVBO;
}

bool ChunkManager::IsChunkVisible(int x, int y)
{
	return true;
}
