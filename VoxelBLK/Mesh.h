#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>

static const int VERTEX_COMPONENT_COUNT = 7;

struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
};

static glm::vec4 MESH_DEFAULT_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addVertex(Vertex v);
	void addVertex(glm::vec3 pos);
	void addVertex(glm::vec3 pos, glm::vec4 color);

	std::vector<Vertex> getVertices();
	GLuint getVAO();
	GLuint getVBO();

	GLfloat* verticesToArray();

private:
	std::vector<Vertex> _vertices;
	GLuint _VBO, _VAO;
	
};