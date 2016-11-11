#include "Chunk.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Chunk::Chunk()
{
  _mesh = new Mesh();
}

Chunk::~Chunk()
{

}

void Chunk::loadChunk(Renderer* renderer)
{
	generateMesh();
	renderer->LoadMesh(_mesh);
}

void Chunk::unloadChunk(Renderer * renderer)
{
	renderer->UnloadMesh(_mesh);
}

void Chunk::renderChunk(Renderer * renderer)
{
	renderer->RenderMesh(_mesh);
}

void Chunk::DEBUG_fillChunk(DEBUG_CHUNK fill)
{
	srand(time(NULL));
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (fill == FILL_ALL)
					_blocks[x][y][z].setActive(true);
				else if (fill == FILL_RANDOM && rand() % 2 + 1 > 1)
				{
					_blocks[x][y][z].setActive(true);
				}
			}
		}
	}	
}

void Chunk::generateMesh()
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (_blocks[x][y][z].isActive() && block_visible(x,y,z)) {
					// Back face
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));

					//Front face
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));

					// Left Face
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));

					// Right face
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));

					//Bottom face
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z));

					// Top face
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z));
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z));
				}
			}
		}
	}
}

bool Chunk::block_visible(int x, int y, int z)
{
	// UNDONE : calculate visiblty block
	if (x > 0 && x < CHUNK_SIZE && y > 0 && y < CHUNK_SIZE && z > 0 && z < CHUNK_SIZE)
	{
		if (_blocks[x + 1][y][z].isActive() && _blocks[x - 1][y][z].isActive()
			&& _blocks[x][y + 1][z].isActive() && _blocks[x][y - 1][z].isActive())
		{
			return false;
		}
		else
			return true;
		
	}
	else
		return true;
}
