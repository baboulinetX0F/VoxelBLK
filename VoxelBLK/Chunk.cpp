#include "Chunk.h"
#include "ChunkManager.h"

#include <stdlib.h>
#include <time.h>

Chunk::Chunk()
{
	
}

Chunk::Chunk(glm::vec2 pos)
{	
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

void Chunk::loadChunk(ChunkManager * manager, Renderer* renderer)
{
	//generateMesh();	
	if (renderer->LoadToManagedVBO(manager->getVBO(), GetVerticesData(), VERTEX_DEFAULT_ATTRIBS,
		_vertices.size() * VERTEX_COMPONENT_COUNT * sizeof(GLfloat)) != -1)
	{
		_loaded = true;
		_state = LOADED;
		_vertices.clear();
		std::cout << "Loaded.\n";
	}
	else {
		_loaded = false;
		_state = MESH_GENERATED;
	}
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
			for (int y = 0; y < CHUNK_SIZE; y++)
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
						if (sqrt((float)(x - CHUNK_SIZE / 2)*(x - CHUNK_SIZE / 2)
							+ (y - CHUNK_SIZE / 2)*(y - CHUNK_SIZE / 2)
							+ (z - CHUNK_SIZE / 2)*(z - CHUNK_SIZE / 2)) <= CHUNK_SIZE / 2)
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

CHUNK_STATE Chunk::getState() const
{
	return _state;
}

unsigned int Chunk::GetVerticesCount() const
{
	return _vertices.size();
}

GLfloat * Chunk::GetVerticesData() const
{
	GLfloat* output = new GLfloat[_vertices.size() * VERTEX_COMPONENT_COUNT];
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		output[(i* VERTEX_COMPONENT_COUNT)] = _vertices.at(i).position.x;
		output[(i*VERTEX_COMPONENT_COUNT) + 1] = _vertices.at(i).position.y;
		output[(i*VERTEX_COMPONENT_COUNT) + 2] = _vertices.at(i).position.z;
		output[(i*VERTEX_COMPONENT_COUNT) + 3] = _vertices.at(i).texCoords.x;
		output[(i*VERTEX_COMPONENT_COUNT) + 4] = _vertices.at(i).texCoords.y;
		output[(i*VERTEX_COMPONENT_COUNT) + 5] = _vertices.at(i).texIndex;
	}
	return output;
}

void Chunk::generateMesh()
{	
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
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			{				
				if (_blocks[x][y][z].isActive() && block_visible(x,y,z)) {					
					BlockFaces nearbyFaces = getBlocksNearby(x, y, z);	
				
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
		for (int y = 0; y < CHUNK_SIZE; y++)
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
	for (int y = 0; y < CHUNK_SIZE; y++)
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

	color = MESH_DEFAULT_COLOR;
	
	GLfloat textureIndex = 2.0f;	

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

		glm::vec2 texCoordsLeftTop = glm::vec2(0.0f, 1.0f * quads[i].h);
		glm::vec2 texCoordsRightTop = glm::vec2(1.0f * quads[i].w, 0.0f);
		glm::vec2 texCoordsLeftBottom = glm::vec2(0.0f, 0.0f);
		glm::vec2 texCoordsRightBottom = glm::vec2(1.0f * quads[i].w, 1.0f * quads[i].h);
		
		// Front face
		if (!back) {
			_vertices.push_back(Vertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), texCoordsRightTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), -0.5f + quads[i].y, -0.5f + quads[i].z), texCoordsRightBottom, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), texCoordsRightTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, -0.5f + quads[i].z), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + quads[i].x, 0.5f + (quads[i].h - 1) + quads[i].y, -0.5f + quads[i].z), texCoordsLeftBottom, textureIndex));
		}

		// Back face
		if (!front) {
			_vertices.push_back(Vertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, 0.5f + quads[i].z), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, 0.5f + quads[i].z), texCoordsRightTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), -0.5f + quads[i].y, 0.5f + quads[i].z), texCoordsRightBottom, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + quads[i].x + (quads[i].w - 1), 0.5f + (quads[i].h - 1) + quads[i].y, 0.5f + quads[i].z), texCoordsRightTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + quads[i].x, -0.5f + quads[i].y, 0.5f + quads[i].z), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + quads[i].x, 0.5f + (quads[i].h - 1) + quads[i].y, 0.5f + quads[i].z), texCoordsLeftBottom, textureIndex));
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
		
		glm::vec2 texCoordsLeftTop = glm::vec2(0.0f, 1.0f * z_quads[i].h);
		glm::vec2 texCoordsRightTop = glm::vec2(1.0f * z_quads[i].w, 0.0f);
		glm::vec2 texCoordsLeftBottom = glm::vec2(0.0f, 0.0f);
		glm::vec2 texCoordsRightBottom = glm::vec2(1.0f * z_quads[i].w, 1.0f * z_quads[i].h);

		// Left Face
		if (!left) {
			_vertices.push_back(Vertex(glm::vec3(-0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), texCoordsRightTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), -0.5f + z_quads[i].x), texCoordsLeftBottom, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + z_quads[i].z, -0.5f + z_quads[i].y, 0.5f + (z_quads[i].w - 1) + z_quads[i].x), texCoordsRightBottom, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(-0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), texCoordsRightTop, textureIndex));
		}

		// Right Face
		if (!right) {
			_vertices.push_back(Vertex(glm::vec3(0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), texCoordsRightTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), -0.5f + z_quads[i].x), texCoordsLeftBottom, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + z_quads[i].z, -0.5f + z_quads[i].y, -0.5f + z_quads[i].x), texCoordsLeftTop, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + z_quads[i].z, -0.5f + z_quads[i].y, 0.5f + (z_quads[i].w - 1) + z_quads[i].x), texCoordsRightBottom, textureIndex));
			_vertices.push_back(Vertex(glm::vec3(0.5f + z_quads[i].z, 0.5f + z_quads[i].y + (z_quads[i].h - 1), 0.5f + (z_quads[i].w - 1) + z_quads[i].x), texCoordsRightTop, textureIndex));
		}
	}

	color = MESH_OCCLUSION_PRIMITIVE_COLOR;

	// TODO : Add neighboors face culling for bottom/top faces
	for (unsigned int i = 0; i < y_quads.size(); i++)
	{
		glm::vec2 texCoordsLeftTop = glm::vec2(0.0f, 1.0f * y_quads[i].h);
		glm::vec2 texCoordsRightTop = glm::vec2(1.0f * y_quads[i].w, 0.0f);
		glm::vec2 texCoordsLeftBottom = glm::vec2(0.0f, 0.0f);
		glm::vec2 texCoordsRightBottom = glm::vec2(1.0f * y_quads[i].w, 1.0f * y_quads[i].h);

		//Bottom face
		_vertices.push_back(Vertex(glm::vec3(-0.5f + y_quads[i].y, -0.5f + y_quads[i].z, -0.5f + y_quads[i].x), texCoordsLeftTop, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), -0.5f + y_quads[i].z, -0.5f + y_quads[i].x), texCoordsLeftBottom, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), -0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), texCoordsRightTop, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), -0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), texCoordsRightTop, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(-0.5f + y_quads[i].y, -0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), texCoordsRightBottom, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(-0.5f + y_quads[i].y, -0.5f + y_quads[i].z, -0.5f + y_quads[i].x), texCoordsLeftTop, textureIndex));

		//Top face
		_vertices.push_back(Vertex(glm::vec3(-0.5f + y_quads[i].y, 0.5f + y_quads[i].z, -0.5f + y_quads[i].x), texCoordsLeftTop, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), 0.5f + y_quads[i].z, -0.5f + y_quads[i].x), texCoordsLeftBottom, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), 0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), texCoordsRightTop, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(0.5f + y_quads[i].y + (y_quads[i].h - 1), 0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), texCoordsRightTop, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(-0.5f + y_quads[i].y, 0.5f + y_quads[i].z, 0.5f + y_quads[i].x + (y_quads[i].w - 1)), texCoordsRightBottom, textureIndex));
		_vertices.push_back(Vertex(glm::vec3(-0.5f + y_quads[i].y, 0.5f + y_quads[i].z, -0.5f + y_quads[i].x), texCoordsLeftTop, textureIndex));

	}

	// Transform vertices positions into world position
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{		
		_vertices[i].position.x += _position.x * CHUNK_SIZE;
		_vertices[i].position.z += _position.y * CHUNK_SIZE;
	}

	_state = MESH_GENERATED;
}

bool Chunk::block_visible(int x, int y, int z)
{		
	if (x > 0 && x < CHUNK_SIZE - 1 && y > 0 && y< CHUNK_SIZE - 1 && z > 0 && z< CHUNK_SIZE - 1) {
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
	if (y == CHUNK_SIZE - 1)
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