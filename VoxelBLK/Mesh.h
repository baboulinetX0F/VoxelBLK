#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>

static const int VERTEX_COMPONENT_COUNT = 3;

struct Vertex
{
	glm::vec3 position;
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	void addVertex(Vertex v);
	void addVertex(glm::vec3 pos);

	std::vector<Vertex> getVertices();
	GLuint getVAO();
	GLuint getVBO();

	GLfloat* verticesToArray();

private:
	std::vector<Vertex> _vertices;
	GLuint _VBO, _VAO;
};