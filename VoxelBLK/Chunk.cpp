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

					// TODO : Move to a new function
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
	// TODO : Move struct away from local context
	struct Quad
	{
		int x;
		int y;
		int z;
		int w;
		int h;		
	};

	glm::vec4 color = MESH_OCCLUSION_PRIMITIVE_COLOR;

	Quad* currentQuad = new Quad();

	// Pass for X-axis quads
	bool x_seg = false;	
	std::vector<Quad> quads;	
	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			{				
				if (_blocks[x][y][z].isActive() && block_visible(x,y,z)) {
					
					BlockFaces nearbyFaces = getBlocksNearby(x, y, z);
					
					//Bottom face
					if (!nearbyFaces.down) {
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
					}

					// Top face
					if (!nearbyFaces.top) {
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
					_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
					}						
					
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

	// Pass for Z-Axis quads
	bool z_seg = false;
	std::vector<Quad> z_quads;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (_blocks[x][y][z].isActive() && block_visible(x, y, z)) {	

					if (!z_seg)
					{
						z_seg = true;
						currentQuad = new Quad();
						currentQuad->w = 1;
						currentQuad->x = z;
						currentQuad->z = x;
						currentQuad->y = y;
						currentQuad->h = 1;
					}
					else
					{
						currentQuad->w += 1;
					}
					_blocksCount++;
				}
				else if (z_seg)
				{
					z_quads.push_back(*currentQuad);
					currentQuad = nullptr;
					z_seg = false;
				}
			}
			if (z_seg)
			{
				z_seg = false;
				if (currentQuad != nullptr)
				{
					z_quads.push_back(*currentQuad);
					currentQuad = nullptr;
				}
			}
		}
		if (z_seg)
		{
			z_seg = false;
			if (currentQuad != nullptr)
			{
				z_quads.push_back(*currentQuad);
				currentQuad = nullptr;
			}
		}
	}

	std::cout << "Quads Count pre-merge : " << quads.size() << std::endl; // Debug Display
	std::cout << "ZQuads Count pre-merge : " << z_quads.size() << std::endl; 

	// Merge all quads (x-axis / z-axis)
	unsigned int i = 0;	
	while(i < quads.size() - 1)
	{
		if ( (quads[i + 1].y == quads[i].y + quads[i].h) && (quads[i + 1].w == quads[i].w) && (quads[i + 1].z == quads[i].z) && (quads[i + 1].x == quads[i].x) )
		{
			quads[i].h += quads[i+1].h;
			quads.erase(quads.begin() + (i + 1));
		}
		else	
			i++;
	}

	i = 0;
	while (i < z_quads.size() - 1)
	{
		if ((z_quads[i + 1].y == z_quads[i].y + z_quads[i].h) && (z_quads[i + 1].w == z_quads[i].w) && (z_quads[i + 1].z == z_quads[i].z) && (z_quads[i + 1].x == z_quads[i].x))
		{
			z_quads[i].h += z_quads[i + 1].h;
			z_quads.erase(z_quads.begin() + (i + 1));
		}
		else
			i++;
	}

	// Debug Display
	std::cout << "Quads Count post-merge : " << quads.size() << std::endl;
	std::cout << "ZQuads Count post-merge : " << z_quads.size() << std::endl; // Debug Display
	/*int tmp = 0;
	for (unsigned int i = 0; i < quads.size(); i++)
	{
		std::cout << "Quad " << i << " : x: " << quads[i].x << " y: " << quads[i].y << " z: " 
			<< quads[i].z  << " w: " << quads[i].w << " h: " << quads[i].h << std::endl;
	}*/

	color = MESH_DEFAULT_COLOR;

	// Transform quads into triangles-based quads
	BlockFaces nearBlocks = BlockFaces{ true,true,true,true,true,true };
	for (unsigned int i = 0; i < quads.size(); i++)
	{
		// Check if a face (quad) can discarded
		bool front = true;
		bool back = true;	
		for (unsigned int x = quads[i].x; x < quads[i].x + quads[i].w; x++)
		{
			for (unsigned int y = quads[i].y; y < quads[i].y + quads[i].h; y++)
			{
				nearBlocks = getBlocksNearby(x, y, quads[i].z);
				if (!nearBlocks.front && front)				
					front = false;	
				if (!nearBlocks.back && back)
					back = false;				
				if (!front && !back)
					break;
			}
			if (!front && !back)
				break;
		}
		
		// Front face
		if (!front) {
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), -0.5f + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), color);
		}

		// Back face
		if (!back) {
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, 0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, 0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), -0.5f + quads[i].y, 0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, 0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, 0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, 0.5f + (quads[i].h - 1) + quads[i].y, 0.5f + quads[i].z), color);
		}
	}
	
	color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	for (unsigned int i = 0; i < z_quads.size(); i++)
	{		
		// Check if a face (quad) can discarded
		bool left = true;
		bool right = true;
		for (unsigned int x = quads[i].x; x < quads[i].x + quads[i].w; x++)
		{
			for (unsigned int y = quads[i].y; y < quads[i].y + quads[i].h; y++)
			{
				nearBlocks = getBlocksNearby(quads[i].z, y, x);
				if (!nearBlocks.left && left)
					left = false;
				if (!nearBlocks.right && right)
					right = false;
				if (!right && !left)
					break;
			}
			if (!right && !left)
				break;
		}
		
		// Left Face
		if (!left) {
			_mesh->addVertex(glm::vec3(-0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(-0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), -0.5f + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(-0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(-0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(-0.5f + z_quads[i].z, -0.5f + z_quads[i].y, 0.5f + (z_quads[i].w - 1) + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(-0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), color);
		}

		// Right Face
		if (!right) {
			_mesh->addVertex(glm::vec3(0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), -0.5f + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(0.5f + z_quads[i].z, -0.5f + z_quads[i].y, 0.5f + (z_quads[i].w - 1) + z_quads[i].x), color);
			_mesh->addVertex(glm::vec3(0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), color);
		}
	}
}

bool Chunk::block_visible(int x, int y, int z)
{	
	if (x > 0 && x < CHUNK_SIZE - 1 && y > 0 && y< WORLD_HEIGHT - 1 && z > 0 && z< CHUNK_SIZE - 1) {
		if (_blocks[x + 1][y][z].isActive() && _blocks[x - 1][y][z].isActive()) {
			if (_blocks[x][y + 1][z].isActive() && _blocks[x][y - 1][z].isActive()) {
				if (_blocks[x][y][z+1].isActive() && _blocks[x][y][z-1].isActive()) {
					return false;
				}
			}
		}
	}
	return true;
}

BlockFaces Chunk::getBlocksNearby(int x, int y, int z)
{	
	BlockFaces output;
	if (y == 0)
		output.down = false;
	else
		output.down = _blocks[x][y - 1][z].isActive();
	if (y == WORLD_HEIGHT - 1)
		output.top = false;
	else
		output.top = _blocks[x][y + 1][z].isActive();
	if (x <= 0)
		output.left = false;
	else
		output.left = _blocks[x - 1][y][z].isActive();
	if (x >= CHUNK_SIZE - 1)
		output.right = false;
	else
		output.right = _blocks[x + 1][y][z].isActive();
	if (z == 0)
		output.front = false;
	else
		output.front = _blocks[x][y][z - 1].isActive();
	if (z == CHUNK_SIZE - 1)
		output.back = false;
	else
		output.back = _blocks[x][y][z + 1].isActive();		
	return output;
}