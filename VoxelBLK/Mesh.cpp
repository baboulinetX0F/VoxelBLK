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

std::vector<Vertex> Mesh::getVertices()
{
	return _vertices;
}

GLuint Mesh::getVAO()
{
	return _VAO;
}

GLuint Mesh::getVBO()
{
	return _VBO;
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
