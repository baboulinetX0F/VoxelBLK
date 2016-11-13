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
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			_chunks[x][y] = new Chunk();
		}
	}
}

void ChunkManager::DEBUG_fillChunks(DEBUG_CHUNK d)
{
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			_chunks[x][y]->DEBUG_fillChunk(d);
		}
	}
}

void ChunkManager::Update(Renderer* renderer)
{
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			if (chunk_visible(x,y) && !_chunks[x][y]->isLoaded())
			{
				_chunks[x][y]->loadChunk(renderer);
			}
			else if (!chunk_visible(x, y))
			{
				_chunks[x][y]->unloadChunk(renderer);
			}
		}
	}
}

void ChunkManager::Render(Renderer * renderer)
{
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, glm::vec3(x*CHUNK_SIZE, 0.0f, y*CHUNK_SIZE));
			_chunks[x][y]->renderChunk(renderer, model);
		}
	}
}

bool ChunkManager::chunk_visible(int x, int y)
{
	return true;
}
