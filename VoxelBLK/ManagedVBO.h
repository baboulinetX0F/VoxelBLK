#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>

#include <stack>
#include <iostream>

struct VertexAttrib
{
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const GLvoid * pointer;

	VertexAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
		const GLvoid * pointer) : index(index), size(size), type(type), normalized(normalized),
		stride(stride), pointer(pointer)
	{

	}
};

class ManagedVBO
{
public:
	ManagedVBO(GLuint VAO, unsigned int blockSize, unsigned int blockCount);
	~ManagedVBO();

	GLuint GetVAO();
	GLuint GetVBO();

	unsigned int LoadData(GLfloat* data, VertexAttrib* attrib);
	void UnloadData(unsigned int index);

private:
	GLuint _VBO, _VAO;
	unsigned int _blockSize;
	unsigned int _bufferSize;
	std::stack<int> _blocksAvailable;
};