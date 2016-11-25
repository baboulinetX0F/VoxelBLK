#include "Chunk.h"

#include <stdlib.h>
#include <time.h>

#include "ChunkManager.h"

Chunk::Chunk()
{
	_mesh = new Mesh();
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
	//generateMesh();
	experimental_genMesh();
	//std::cout << "CHUNK : " << _blocksCount << " blocks generated\n";
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

void Chunk::generateChunk(CHUNK_GEN_MODE genMode, ChunkManager* _manager)
{
	if (genMode == GEN_PERLIN)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				int cell_height = _manager->getNoiseValue(x, z);
				for (int y = 0; y < cell_height; y++)
				{
					_blocks[x][y][z].setActive(true);
				}
			}
		}
	}
	else {
		srand(time(NULL));
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int y = 0; y < WORLD_HEIGHT; y++)
			{
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					if (genMode == GEN_FULL)
						_blocks[x][y][z].setActive(true);
					else if (genMode == GEN_RANDOM)
					{
						_blocks[x][y][z].setActive(rand() % 2 + 1 > 1 ? true : false);
					}
					else if (genMode == GEN_SPHERE)
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
				// TODO : Integrate Greedy meshing
				if (block_visible(x, y, z) && _blocks[x][y][z].isActive()) {
					glm::vec4 color = glm::vec4((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);

					bool top,down,left,right,front,back;
					if (y == 0)						
						down = false;
					else
						down = _blocks[x][y - 1][z].isActive();
					if (y == WORLD_HEIGHT - 1)
						top = false;
					else						
						top = _blocks[x][y + 1][z].isActive();
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
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
					}

					//Front face
					if (!front) {
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
					}

					// Left Face
					if (!left) {
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
					}

					// Right face
					if (!right) {
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
					}
					
					//Bottom face
					if (!down) {
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
					}

					// Top face
					if (!top) {
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
					}
					_blocksCount++;
				}
			}
		}
	}
}

void Chunk::experimental_genMesh()
{
	struct Quad
	{
		int x;
		int y;
		int z;
		int w;
		int h;		
	};

	bool x_seg = false;
	Quad* currentQuad = new Quad();
	std::vector<Quad> quads;

	glm::vec4 color = glm::vec4((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);

	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				if (block_visible(x, y, z) && _blocks[x][y][z].isActive()) {
					bool top, down, left, right, front, back;
					if (y == 0)
						down = false;
					else
						down = _blocks[x][y - 1][z].isActive();
					if (y == WORLD_HEIGHT - 1)
						top = false;
					else
						top = _blocks[x][y + 1][z].isActive();
					if (x == 0)
						left = false;
					else
						left = _blocks[x - 1][y][z].isActive();
					if (x == CHUNK_SIZE - 1)
						right = false;
					else
						right = _blocks[x + 1][y][z].isActive();
					if (z == 0)
						back = false;
					else
						back = _blocks[x][y][z - 1].isActive();
					if (z == CHUNK_SIZE - 1)
						front = false;
					else
						front = _blocks[x][y][z + 1].isActive();

					// TODO : Fix face discard for experimental meshing
					// Left Face
					//if (!left) {
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
					//}

					// Right face
					//if (!right) {
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
					//}

					//Bottom face
					//if (!down) {
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
					//}

					// Top face
					//if (!top) {
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
					//}

						
					if (z == 0 || true) {
						if (!x_seg)
						{
							x_seg = true;
							currentQuad = new Quad();
							currentQuad->w = 1;
							currentQuad->x = x;
							currentQuad->z = z;
							currentQuad->y = y;
							currentQuad->h = 1;
						}
						else
						{
							currentQuad->w += 1;
						}

						_blocksCount++;
					}
					else if (x_seg)
					{
						quads.push_back(*currentQuad);
						currentQuad = nullptr;
						x_seg = false;
					}
				}				
			}
			if (x_seg)
			{
				x_seg = false;
				if (currentQuad != nullptr)
				{
					quads.push_back(*currentQuad);
					currentQuad = nullptr;
				}
			}
		}
		if (x_seg)
		{
			x_seg = false;
			if (currentQuad != nullptr)
			{
				quads.push_back(*currentQuad);
				currentQuad = nullptr;
			}
		}
	}

	std::cout << "Quad Count pre-merge : " << quads.size() << std::endl; // Debug Display

	// UNDONE : Merge Quads
	// TODO : Fix the height problem for front/back face
	int i = 0;	
	while(i < quads.size() - 1)
	{
		if (quads[i + 1].y == quads[i].y + quads[i].h && quads[i + 1].w == quads[i].w && quads[i + 1].z == quads[i].z && quads[i + 1].x == quads[i].x)
		{
			quads[i].h += 1;
			quads.erase(quads.begin() + (i + 1));
		}
		else	
			i++;
	}

	// Debug Display
	std::cout << "Quad Count post-merge : " << quads.size() << std::endl;
	int tmp = 0;
	for (unsigned int i = 0; i < quads.size(); i++)
	{
		std::cout << "Quad " << i << " : x: " << quads[i].x << " y: " << quads[i].y << " w: " << quads[i].w << " h: " << quads[i].h << std::endl;
	}

	color = MESH_DEFAULT_COLOR;

	// Transform quads into triangles-based quads
	for (unsigned int i = 0; i < quads.size(); i++)
	{
		// Back face
		_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(0.5f + quads[i].x + quads[i].w, 0.5f + quads[i].h + quads[i].y, -0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(0.5f + quads[i].x + quads[i].w, -0.5f + quads[i].y, -0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(0.5f + quads[i].x + quads[i].w, 0.5f + quads[i].h + quads[i].y, -0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, 0.5f + quads[i].h + quads[i].y, -0.5f + quads[i].z), color);		

		// Front face
		_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, 0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(0.5f + quads[i].x + quads[i].w, 0.5f + quads[i].h + quads[i].y, 0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(0.5f + quads[i].x + quads[i].w, -0.5f + quads[i].y, 0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(0.5f + quads[i].x + quads[i].w, 0.5f + quads[i].h + quads[i].y, 0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, 0.5f + quads[i].z), color);
		_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, 0.5f + quads[i].h + quads[i].y, 0.5f + quads[i].z), color);		
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
