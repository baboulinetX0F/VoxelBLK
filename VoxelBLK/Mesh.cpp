#include "Mesh.h"

#include <iostream>

Mesh::Mesh()
{
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);	;
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
	_vertices.push_back(Vertex{ pos });
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
	GLfloat* output = new GLfloat[_vertices.size() * 3];
	for (int i = 0; i < _vertices.size(); i++)
	{
		output[(i*3)] = _vertices.at(i).position.x;		
		output[(i*3) + 1] = _vertices.at(i).position.y;
		output[(i*3) + 2] = _vertices.at(i).position.z;
	}
	return output;
}
