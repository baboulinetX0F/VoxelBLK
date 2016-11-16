#include "Mesh.h"

#include <iostream>

Mesh::Mesh()
{
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);	
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}

void Mesh::addVertex(Vertex v)
{
	_vertices.push_back(v);
}

void Mesh::addVertex(glm::vec3 pos)
{
	_vertices.push_back(Vertex{ pos, MESH_DEFAULT_COLOR });
}

void Mesh::addVertex(glm::vec3 pos, glm::vec4 color)
{
	_vertices.push_back(Vertex{ pos, color });
}

const std::vector<Vertex> Mesh::getVertices()
{
	return _vertices;
}

const GLuint Mesh::getVAO()
{
	return _VAO;
}

const GLuint Mesh::getVBO()
{
	return _VBO;
}

Mesh * Mesh::getOcclusionPrimitive()
{
	return _occlusionPrimitive;
}

GLfloat * Mesh::verticesToArray()
{
	GLfloat* output = new GLfloat[_vertices.size() * VERTEX_COMPONENT_COUNT];
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		output[(i* VERTEX_COMPONENT_COUNT)] = _vertices.at(i).position.x;
		output[(i*VERTEX_COMPONENT_COUNT) + 1] = _vertices.at(i).position.y;
		output[(i*VERTEX_COMPONENT_COUNT) + 2] = _vertices.at(i).position.z;
		output[(i* VERTEX_COMPONENT_COUNT) + 3] = _vertices.at(i).color.r;
		output[(i*VERTEX_COMPONENT_COUNT) + 4] = _vertices.at(i).color.g;
		output[(i*VERTEX_COMPONENT_COUNT) + 5] = _vertices.at(i).color.b;
		output[(i*VERTEX_COMPONENT_COUNT) + 6] = _vertices.at(i).color.a;
	}
	return output;
}

void Mesh::generateOcclusionPrimitve()
{	
	if (_occlusionPrimitive == nullptr)
		_occlusionPrimitive = new Mesh();

	float x_min = 0.0f, x_max = 0.0f, y_min = 0.0f, y_max = 0.0f, z_min = 0.0f, z_max = 0.0f;
	glm::vec3 tmp;	
	for (unsigned int i = 0; i < _vertices.size(); i++)
	{
		tmp = _vertices[i].position;
		if (tmp.x < x_min)
			x_min = tmp.x;
		if (tmp.x > x_max)
			x_max = tmp.x;
		if (tmp.y < y_min)
			y_min = tmp.y;
		if (tmp.y > y_max)
			y_max = tmp.y;
		if (tmp.z < z_min)
			z_min = tmp.z;
		if (tmp.z > z_max)
			z_max = tmp.z;
	}	
	glm::vec4 color = MESH_OCCLUSION_PRIMITIVE_COLOR;

	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_min), color);

	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_max), color);

	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_max), color);

	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_max), color);

	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_min, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_min, z_min), color);

	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_max, y_max, z_max), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_min), color);
	_occlusionPrimitive->addVertex(glm::vec3(x_min, y_max, z_max), color);

}
