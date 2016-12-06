#include "Chunk.h"
#include "ChunkManager.h"

#include <stdlib.h>
#include <time.h>

Chunk::Chunk()
{
	_mesh = new Mesh();
}

Chunk::Chunk(glm::vec2 pos)
{
	_mesh = new Mesh();
	_position = pos;
}

Chunk::~Chunk()
{	
}

bool Chunk::isLoaded()
{
	return _loaded;
}

bool Chunk::isEmpty()
{
	return _empty;
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

GLuint Chunk::getVAOID()
{
	return _mesh->getVAO();
}

void Chunk::generateChunk(CHUNK_GEN_MODE genMode, ChunkManager* _manager)
{
	if (genMode == GEN_PERLIN)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				float x_world = static_cast<float>(x + (_position.x * CHUNK_SIZE)) / 64;
				float z_world = static_cast<float>(z + (_position.y * CHUNK_SIZE)) / 64;
				int cell_height = _manager->getNoiseValue(x_world, z_world);
				if (cell_height >= CHUNK_SIZE - 1) cell_height = CHUNK_SIZE - 1;
				for (int y = 0; y < cell_height; y++)
				{
					_blocks[x][y][z].setActive(true);
					if (_empty)
						_empty = false;
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
					{
						_blocks[x][y][z].setActive(true);
						if (_empty)
							_empty = false;
					}
					else if (genMode == GEN_RANDOM)
					{
						_blocks[x][y][z].setActive(rand() % 2 + 1 > 1 ? true : false);
						if (_empty)
							_empty = false;
					}
					else if (genMode == GEN_SPHERE)
					{
						if (sqrt((float)(x - CHUNK_SIZE / 2)*(x - CHUNK_SIZE / 2) + (y - WORLD_HEIGHT / 2)*(y - WORLD_HEIGHT / 2) + (z - CHUNK_SIZE / 2)*(z - CHUNK_SIZE / 2)) <= CHUNK_SIZE / 2)
						{
							_blocks[x][y][z].setActive(true);
							if (_empty)
								_empty = false;
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
				if (block_visible(x, y, z) && _blocks[x][y][z].isActive()) {
					glm::vec4 color = glm::vec4((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, 1.0f);

					BlockFaces nearbyFaces = getBlocksNearby(x, y, z);

					// Back face
					if (!nearbyFaces.back) {
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
					}

					//Front face
					if (!nearbyFaces.front) {
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
					}

					// Left Face
					if (!nearbyFaces.left) {
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, -0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(-0.5f + x, 0.5f + y, 0.5f + z), color);
					}

					// Right face
					if (!nearbyFaces.right) {
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, -0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, 0.5f + y, 0.5f + z), color);
						_mesh->addVertex(glm::vec3(0.5f + x, -0.5f + y, 0.5f + z), color);
					}
					
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
					/*if (!nearbyFaces.down) {
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
					}*/						
				
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

	// Pass for Z-Axis quads
	bool y_seg = false;
	std::vector<Quad> y_quads;
	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (_blocks[x][y][z].isActive() && block_visible(x, y, z)) {

					if (!y_seg)
					{
						y_seg = true;
						currentQuad = new Quad();
						currentQuad->w = 1;
						currentQuad->x = z;
						currentQuad->z = y;
						currentQuad->y = x;
						currentQuad->h = 1;
					}
					else
					{
						currentQuad->w += 1;
					}					
				}
				else if (y_seg)
				{
					y_quads.push_back(*currentQuad);
					currentQuad = nullptr;
					y_seg = false;
				}
			}
			if (y_seg)
			{
				y_seg = false;
				if (currentQuad != nullptr)
				{
					y_quads.push_back(*currentQuad);
					currentQuad = nullptr;
				}
			}
		}
		if (y_seg)
		{
			y_seg = false;
			if (currentQuad != nullptr)
			{
				y_quads.push_back(*currentQuad);
				currentQuad = nullptr;
			}
		}
	}

	/* Debug Quads count Display
	std::cout << "XQuads Count pre-merge : " << quads.size() << std::endl; 
	std::cout << "ZQuads Count pre-merge : " << z_quads.size() << std::endl; 
	std::cout << "YQuads Count pre-merge : " << y_quads.size() << std::endl;
	*/

	// TODO : Rewrite merge algorithm (to test all merges possible)
	// Merge all quads (top-down/left-right/front-back)

	// front/back quads merging
	unsigned int i_nextquad = 0;
	if (quads.size() > 1) {
		while (i_nextquad < quads.size() - 1)
		{
			if ((quads[i_nextquad + 1].y == quads[i_nextquad].y + quads[i_nextquad].h)
				&& (quads[i_nextquad + 1].w == quads[i_nextquad].w)
				&& (quads[i_nextquad + 1].z == quads[i_nextquad].z)
				&& (quads[i_nextquad + 1].x == quads[i_nextquad].x))
			{
				quads[i_nextquad].h += quads[i_nextquad + 1].h;
				quads.erase(quads.begin() + (i_nextquad + 1));
			}
			else
				i_nextquad++;
		}
	}

	// left/right quads merging
	i_nextquad = 0;
	if (z_quads.size() > 1) {
		while (i_nextquad < z_quads.size() - 1)
		{
			if ((z_quads[i_nextquad + 1].y == z_quads[i_nextquad].y + z_quads[i_nextquad].h)
				&& (z_quads[i_nextquad + 1].w == z_quads[i_nextquad].w)
				&& (z_quads[i_nextquad + 1].z == z_quads[i_nextquad].z)
				&& (z_quads[i_nextquad + 1].x == z_quads[i_nextquad].x))
			{
				z_quads[i_nextquad].h += z_quads[i_nextquad + 1].h;
				z_quads.erase(z_quads.begin() + (i_nextquad + 1));
			}
			else
				i_nextquad++;
		}
	}

	// top/down quads merging
	i_nextquad = 0;
	if (y_quads.size() > 1) {
		while (i_nextquad < y_quads.size() - 1)
		{
			if ((y_quads[i_nextquad + 1].y == y_quads[i_nextquad].y + y_quads[i_nextquad].h)
				&& (y_quads[i_nextquad + 1].w == y_quads[i_nextquad].w)
				&& (y_quads[i_nextquad + 1].z == y_quads[i_nextquad].z)
				&& (y_quads[i_nextquad + 1].x == y_quads[i_nextquad].x))
			{
				y_quads[i_nextquad].h += y_quads[i_nextquad + 1].h;
				y_quads.erase(y_quads.begin() + (i_nextquad + 1));
			}
			else
				i_nextquad++;
		}
	}

	/* Debug Quads count post-merge Display
	std::cout << "XQuads Count post-merge : " << quads.size() << std::endl;
	std::cout << "ZQuads Count post-merge : " << z_quads.size() << std::endl; 
	std::cout << "YQuads Count post-merge : " << y_quads.size() << std::endl; 		
	
	/*for (unsigned int i = 0; i < quads.size(); i++)
	{
		std::cout << "Quad " << i << " : x: " << quads[i].x << " y: " << quads[i].y << " z: " 
			<< quads[i].z  << " w: " << quads[i].w << " h: " << quads[i].h << std::endl;
	}*/

	color = MESH_DEFAULT_COLOR;

	// Transform quads into triangles-based quads and send vertices to the mesh object
	BlockFaces nearBlocks = BlockFaces{ true,true,true,true,true,true };
	for (unsigned int i = 0; i < quads.size(); i++)
	{
		// Check if a face (quad) can discarded
		bool front = true;
		bool back = true;	
		for (int x = quads[i].x; x < quads[i].x + quads[i].w; x++)
		{
			for (int y = quads[i].y; y < quads[i].y + quads[i].h; y++)
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
		if (!back) {
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), -0.5f + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), color);
			_mesh->addVertex(glm::vec3(-0.5f + quads[i].x, 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), color);
		}

		// Back face
		if (!front) {
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
		for (int x = z_quads[i].x; x < z_quads[i].x + z_quads[i].w; x++)
		{
			for (int y = z_quads[i].y; y < z_quads[i].y + z_quads[i].h; y++)
			{
				nearBlocks = getBlocksNearby(z_quads[i].z, y, x);
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

	color = MESH_OCCLUSION_PRIMITIVE_COLOR;

	// TODO : Add neighboors face culling for bottom/top faces
	for (unsigned int i = 0; i < y_quads.size(); i++)
	{
		//Bottom face
		_mesh->addVertex(glm::vec3(-0.5f + y_quads[i].y, -0.5f + y_quads[i].z, -0.5f + y_quads[i].x), color);
		_mesh->addVertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), -0.5f + y_quads[i].z, -0.5f + y_quads[i].x), color);
		_mesh->addVertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), -0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), color);
		_mesh->addVertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), -0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), color);
		_mesh->addVertex(glm::vec3(-0.5f + y_quads[i].y, -0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), color);
		_mesh->addVertex(glm::vec3(-0.5f + y_quads[i].y, -0.5f + y_quads[i].z, -0.5f + y_quads[i].x), color);

		//Top face
		_mesh->addVertex(glm::vec3(-0.5f + y_quads[i].y, 0.5f + y_quads[i].z, -0.5f + y_quads[i].x), color);
		_mesh->addVertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), 0.5f + y_quads[i].z, -0.5f + y_quads[i].x), color);
		_mesh->addVertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), 0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), color);
		_mesh->addVertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), 0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), color);
		_mesh->addVertex(glm::vec3(-0.5f + y_quads[i].y, 0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), color);
		_mesh->addVertex(glm::vec3(-0.5f + y_quads[i].y, 0.5f + y_quads[i].z, -0.5f + y_quads[i].x), color);

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
		output.back = false;
	else
		output.back = _blocks[x][y][z - 1].isActive();
	if (z == CHUNK_SIZE - 1)
		output.front = false;
	else
		output.front = _blocks[x][y][z + 1].isActive();		
	return output;	
}