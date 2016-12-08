#include "ManagedVBO.h"

ManagedVBO::ManagedVBO(GLuint VAO, unsigned int blockSize, unsigned int blockCount)
{
	glGenBuffers(1, &_VBO);
	_VAO = VAO;
	_blockSize = blockSize;
	_bufferSize = blockSize * blockCount;

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _bufferSize, nullptr, GL_STATIC_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Fill stack of available data blocks
	for (unsigned int i = 0; i < blockCount; i++)
		_blocksAvailable.push(i);
}

ManagedVBO::~ManagedVBO()
{
	glDeleteBuffers(1, &_VBO);
}

GLuint ManagedVBO::GetVAO()
{
	return _VAO;
}

GLuint ManagedVBO::GetVBO()
{
	return _VBO;
}

unsigned int ManagedVBO::LoadData(GLfloat * data, VertexAttrib* attrib)
{
	if (sizeof(data) > _blockSize)
		std::cerr << "WARNING : The data passed to load into the managed VBO is too large. Risk of losing data\n";
	if (_blocksAvailable.size() >= 0)
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferSubData(GL_ARRAY_BUFFER, _blockSize * _blocksAvailable.top(), _blockSize, data);

		if (attrib != nullptr) {
			int i = 0;
			for (VertexAttrib* ptr = &attrib[i]; ptr != nullptr; ptr = &attrib[i])
			{
				glVertexAttribPointer(ptr->index, ptr->size, ptr->type, ptr->normalized,
					ptr->stride, ptr->pointer);
				glEnableVertexAttribArray(ptr->index);
				i++;
			}
		}

		_blocksAvailable.pop();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	else
		return -1;
}

void ManagedVBO::UnloadData(unsigned int index)
{
	_blocksAvailable.push(index);
}
