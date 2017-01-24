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
	try
	{
		auto status = _calculationThread.wait_for(std::chrono::seconds(0));
		if (status == std::future_status::ready) {
			auto func = std::bind(&ChunkManager::UpdateMeshes, this);
			_calculationThread = std::async(std::launch::async, func, 0);
		}
	}
	catch (const std::future_error& e) {
		std::cout << "No async thread launched\n";
		auto func = std::bind(&ChunkManager::UpdateMeshes, this);
		_calculationThread = std::async(std::launch::async, func, 0);
	}
			
	for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
	{
		for (int x = 0; x < CHUNKS_NUMBER_X; x++)  
		{
			if (!IsChunkVisible(x, y) && _chunks[x][y]->getState() == CHUNK_STATE::LOADED)
			{
				UnloadChunk(_chunks[x][y]);
			}
			if (IsChunkVisible(x, y) && !_chunks[x][y]->isEmpty() && _chunks[x][y]->getState() == CHUNK_STATE::MESH_GENERATED) {
				std::cout << "Loading Chunk...\n";
				_chunks[x][y]->loadChunk(this, renderer);
			}
					
		}
	}
}

void ChunkManager::Render(Renderer * renderer)
{
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

void ChunkManager::UnloadChunk(Chunk* chunk)
{
	//_chunksVBO->UnloadData()
}

void ChunkManager::UpdateMeshes()
{
	for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
	{
		for (int x = 0; x < CHUNKS_NUMBER_X; x++)
		{
			if (IsChunkVisible(x, y) && !_chunks[x][y]->isEmpty() && _chunks[x][y]->getState() == CHUNK_STATE::UNLOADED)
			{				
				std::cout << "Generating Chunk...\n";
				_chunks[x][y]->generateMesh();
			}			
		}
	}
}

void ChunkManager::UpdateVBO(Renderer* renderer)
{		
	for (int y = 0; y < CHUNKS_NUMBER_Y; y++)
	{
		for (int x = 0; x < CHUNKS_NUMBER_X; x++)
		{
			if (!IsChunkVisible(x, y) && _chunks[x][y]->getState() == CHUNK_STATE::LOADED)
			{
				UnloadChunk(_chunks[x][y]);
			}
			if (IsChunkVisible(x, y) && !_chunks[x][y]->isEmpty() && _chunks[x][y]->getState() == CHUNK_STATE::MESH_GENERATED) {
				std::cout << "Loading Chunk...\n";
				_chunks[x][y]->loadChunk(this, renderer);
			}

		}
	}
}
