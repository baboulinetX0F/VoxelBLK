#include "Chunk.h"

#include <stdlib.h>
#include <time.h>

Chunk::Chunk()
{
  _mesh = new Mesh();
  _blocksCount = 0;
  _loaded = false;
}

Chunk::~Chunk()
{

}

bool Chunk::isLoaded()
{
	return _loaded;
}

void Chunk::loadChunk(Renderer* renderer)
{
	generateMesh();	
	//generateOcclusionMesh(renderer);
	std::cout << "CHUNK : " << _blocksCount << " blocks generated\n";
	//renderer->LoadMesh(_occlusionMesh);
	renderer->LoadMesh(_mesh);
	_loaded = true;
}

void Chunk::unloadChunk(Renderer * renderer)
{
	renderer->UnloadMesh(_mesh);
	_loaded = false;
}

void Chunk::renderChunk(Renderer * renderer)
{
	renderer->RenderMesh(_mesh);
}

void Chunk::renderChunk(Renderer * renderer, glm::mat4 model)
{
	renderer->RenderMesh(_mesh,model);
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
				else if (fill == FILL_RANDOM)
				{
					if (rand()%2 + 1 > 1)
						_blocks[x][y][z].setActive(true);
					else
						_blocks[x][y][z].setActive(false);
				}
				else if (fill == FILL_SPHERE)
				{
					if (sqrt((float)(x - CHUNK_SIZE / 2)*(x - CHUNK_SIZE / 2) + (y - WORLD_HEIGHT / 2)*(y - WORLD_HEIGHT / 2) + (z - CHUNK_SIZE / 2)*(z - CHUNK_SIZE / 2)) <= CHUNK_SIZE / 2)
					{
						_blocks[x][y][z].setActive(true);						
					}
				}
					
			}
		}
	}	
}


void Chunk::generateMesh()
{	
	srand(time(NULL));
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				// TODO : Merge vertices of blocks next to each other
				if (block_visible(x, y, z) && _blocks[x][y][z].isActive()) {

					//glm::vec4 color = MESH_DEFAULT_COLOR;
					glm::vec4 color = glm::vec4((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);

					bool top,down,left,right,front,back;
					if (y == 0)
						top = false;
					else
						top = _blocks[x][y - 1][z].isActive();
					if (y == WORLD_HEIGHT - 1)
						down = false;
					else
						down = _blocks[x][y + 1][z].isActive();
					if (x == 0)
						left = false;
					else
						left = _blocks[x-1][y][z].isActive();
					if (x == CHUNK_SIZE - 1)
						right = false;
					else
						right = _blocks[x+1][y][z].isActive();
					if (z == 0)
						back = false;
					else
						back = _blocks[x][y][z-1].isActive();
					if (z == CHUNK_SIZE - 1)
						front = false;
					else
						front = _blocks[x][y][z+1].isActive();

					// Back face
					if (!back) {
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);
					}

					//Front face
					if (!front) {
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);
					}

					// Left Face
					if (!left) {
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);
					}

					// Right face
					if (!right) {
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);
					}
					
					//Bottom face
					if (!down) {
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f - y, -0.5f + z), color);
					}

					// Top face
					if (!top) {
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f - y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f - y, 0.5f + z), color);
					}
					_blocksCount++;
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
