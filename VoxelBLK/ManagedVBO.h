#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm.hpp>

#include <stack>
#include <vector>
#include <tuple>
#include <map>
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
	{ }
};

class ManagedVBO
{
public:
	ManagedVBO(GLuint VAO, unsigned int blockSize, unsigned int blockCount);
	~ManagedVBO();

	GLuint GetVAO();
	GLuint GetVBO();
	const unsigned int GetCurrentMax();

	unsigned int LoadData(GLfloat* data, VertexAttrib* attrib, unsigned int size);
	void LoadDataExp(GLfloat* data);
	void UnloadData(unsigned int index);

private:
	GLuint _VBO, _VAO;
	unsigned int _blockSize;
	unsigned int _bufferSize;
	unsigned int _currentMax;
	std::stack<int> _blocksAvailable;

	// item 1 : block_index | item 2 : block_size	
	std::vector<std::map<int, unsigned int>> _blocks;
};