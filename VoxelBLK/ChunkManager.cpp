#include "ChunkManager.h"


ChunkManager::ChunkManager()
{
	init();
}


ChunkManager::~ChunkManager()
{
}

void ChunkManager::init()
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

void ChunkManager::generateChunks(CHUNK_GEN_MODE d)
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
	for (int x = 0; x < CHUNKS_NUMBER_X; x++)
	{
		for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
		{
			if (chunk_visible(x,y) && !_chunks[x][y]->isLoaded() && !_chunks[x][y]->isEmpty())
			{
				_chunks[x][y]->loadChunk(renderer);
			}			
			else if (!chunk_visible(x, y))
			{
				_chunks[x][y]->unloadChunk(renderer);
			}
			if (!_chunks[x][y]->isLoaded() && _chunks[x][y]->isEmpty())
			{
				std::cout << "VAO : " << _chunks[x][y]->getVAOID() << std::endl;
			}
		}
	}
}

void ChunkManager::Render(Renderer * renderer)
{
	for (int x = 0; x < CHUNKS_NUMBER_X; x++)
	{
		for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
		{
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, glm::vec3(x*CHUNK_SIZE, 0.0f, y*CHUNK_SIZE));
			_chunks[x][y]->renderChunk(renderer, model);
		}
	}
}

int ChunkManager::getNoiseValue(float x, float z)
{	
	return static_cast<int>(_simplex.noise(x,z) * 16);
}

bool ChunkManager::chunk_visible(int x, int y)
{
	return true;
}
