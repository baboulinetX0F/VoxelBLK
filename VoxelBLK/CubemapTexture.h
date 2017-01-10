#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <SOIL.h>

#include <vector>

class CubemapTexture
{
public:
	CubemapTexture(const char* topFile, const char* bottomFile, const char* leftFile ,
		const char* rightFile, const char* frontFile, const char* backFile);
	~CubemapTexture();

	bool Load();
	void Bind();

	const GLuint GetTextureObject();

private:
	GLuint _texObject;
	std::vector<const char*> _faceFiles;
};

