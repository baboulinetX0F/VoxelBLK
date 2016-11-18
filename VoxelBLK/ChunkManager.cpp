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
	// TODO : Move to a new method initPerlin
	_noise.SetFrequency(1);
	_noise.SetLacunarity(2.5);
	_noise.SetPersistence(1);
}

void ChunkManager::DEBUG_fillChunks(CHUNK_GEN_MODE d)
{
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			_chunks[x][y]->generateChunk(d, this);
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

int ChunkManager::getNoiseValue(double x, double z)
{
	return _noise.GetValue(x,z,0) * WORLD_HEIGHT;
}

bool ChunkManager::chunk_visible(int x, int y)
{
	return true;
}
