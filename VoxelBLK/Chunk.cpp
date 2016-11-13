#include "Chunk.h"

#include <stdlib.h>
#include <time.h>

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
	std::cout << "CHUNK : " << blocksGenerated << " blocks generated\n";
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
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (fill == FILL_ALL)
					_blocks[x][y][z].setActive(true);
				else if (fill = FILL_RANDOM)
				{
					if (rand()%2 + 1 > 1)
						_blocks[x][y][z].setActive(true);
					else
						_blocks[x][y][z].setActive(false);
				}
					
			}
		}
	}	
}

void Chunk::generateMesh()
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				// TODO : Merge vertices of blocks next to each other
				if (block_visible(x, y, z) && _blocks[x][y][z].isActive()) {
					
					glm::vec4 color = MESH_DEFAULT_COLOR;
					
					// Back face
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z),color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);

					//Front face
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);

					// Left Face
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);

					// Right face
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);

					//Bottom face
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);

					// Top face
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);

					blocksGenerated++;
				}
			}
		}
	}
}

bool Chunk::block_visible(int x, int y, int z)
{
	// UNDONE : Calculate Block Visibility for y & z axis
	if (x > 0 && x < CHUNK_SIZE - 1 && y > 0 && y< WORLD_HEIGHT - 1 && z > 0 && z< CHUNK_SIZE - 1) {
		if (_blocks[x + 1][y][z].isActive() && _blocks[x - 1][y][z].isActive()) {
			//std::cout << "BLOCK " << x << ":" << y << ":" << z << " SURROUNDED\n";
			return false;
		}
	}
	return true;
}
