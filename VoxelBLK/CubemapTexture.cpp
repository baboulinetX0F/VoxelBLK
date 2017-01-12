#include "CubemapTexture.h"


CubemapTexture::CubemapTexture(const char* topFile, const char* bottomFile, const char* leftFile,
	const char* rightFile, const char* frontFile, const char* backFile)
{
	_faceFiles.push_back(rightFile);
	_faceFiles.push_back(leftFile);
	_faceFiles.push_back(topFile);
	_faceFiles.push_back(bottomFile);
	_faceFiles.push_back(backFile);
	_faceFiles.push_back(frontFile);
}


CubemapTexture::~CubemapTexture()
{
}

bool CubemapTexture::Load()
{
	glGenTextures(1, &_texObject);

	int width, height;
	unsigned char* image;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texObject);
	for (int i = 0; i < _faceFiles.size(); i++)
	{
		image = SOIL_load_image(_faceFiles[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

void CubemapTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texObject);
}

const GLuint CubemapTexture::GetTextureObject()
{
	return _texObject;
}
