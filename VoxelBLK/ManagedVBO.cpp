#include "ManagedVBO.h"

ManagedVBO::ManagedVBO(GLuint VAO, unsigned int blockSize, unsigned int blockCount)
{
	glGenBuffers(1, &_VBO);
	_VAO = VAO;
	_blockSize = blockSize;
	_bufferSize = blockSize * blockCount;
	
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, _bufferSize, NULL, GL_STREAM_DRAW);	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Fill stack of available data blocks
	for (unsigned int i = 0; i < blockCount; i++)
		_blocksAvailable.push((blockCount -1) - i);	
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

const unsigned int ManagedVBO::GetBlockSize()
{
	return _blockSize;
}

unsigned int ManagedVBO::LoadData(GLfloat * data, VertexAttrib* attrib, unsigned int size)
{
	if (sizeof(data) > _blockSize)
		std::cout << "WARNING : The data passed to load into the managed VBO is too large. Risk of losing data\n";
	if (_blocksAvailable.size() > 0)
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferSubData(GL_ARRAY_BUFFER, _blocksAvailable.top() * _blockSize , size, data);
		if (attrib != nullptr) {		
			glVertexAttribPointer(attrib[0].index, attrib[0].size, attrib[0].type, attrib[0].normalized,
				attrib[0].stride, attrib[0].pointer);
			glEnableVertexAttribArray(attrib[0].index);
			glVertexAttribPointer(attrib[1].index, attrib[1].size, attrib[1].type, attrib[1].normalized,
				attrib[1].stride, attrib[1].pointer);
			glEnableVertexAttribArray(attrib[1].index);
			glVertexAttribPointer(attrib[2].index, attrib[2].size, attrib[2].type, attrib[2].normalized,
				attrib[2].stride, attrib[2].pointer);
			glEnableVertexAttribArray(attrib[2].index);
			
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		_blocks[_blocksAvailable.top()] = size;
		_blocksAvailable.pop();
		return 0;
	}
	else
		return -1;
}

void ManagedVBO::UnloadData(unsigned int index)
{
	_blocksAvailable.push(index);
}

