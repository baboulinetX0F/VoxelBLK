#pragma once

#include <vector>

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>

#include "ManagedVBO.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	GLfloat texIndex;
	
	Vertex(glm::vec3 pos, glm::vec2 texCoord, GLfloat texind) : position(pos), texCoords(texCoord),
	texIndex(texind) { }
};

static const int VERTEX_COMPONENT_COUNT = 6;
static const int VERTEX_SIZE = 6 * sizeof(GLfloat);
static VertexAttrib VERTEX_DEFAULT_ATTRIBS[] =
{
	VertexAttrib(0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*)0),
	VertexAttrib(1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*)(3*sizeof(GLfloat))),
	VertexAttrib(2, 1, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (GLvoid*)(5 * sizeof(GLfloat)))
};

static const glm::vec4 MESH_DEFAULT_COLOR = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
static const glm::vec4 MESH_OCCLUSION_PRIMITIVE_COLOR = glm::vec4(0.95f, 0.53f, 0.25f, 0.5f);

class Mesh
{
public:
	Mesh();
	Mesh(GLuint* vao);
	~Mesh();

	void addVertex(Vertex v);
	void addVertex(glm::vec3 pos);
	void addVertex(glm::vec3 pos, glm::vec4 color);

	const std::vector<Vertex> getVertices();
	const GLuint getVAO();
	const GLuint getVBO();
	Mesh* getOcclusionPrimitive();

	GLfloat* verticesToArray();
	void generateOcclusionPrimitve();

private:
	std::vector<Vertex> _vertices;
	GLuint _VBO, _VAO;
	Mesh* _occlusionPrimitive;
	
};