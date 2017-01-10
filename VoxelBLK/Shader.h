#pragma once

#include <iostream>
#include <fstream>
#include <map>

#include <GL\glew.h>

class Shader
{
public:
	GLuint _program;	// Id of the shader program
	Shader(const GLchar* vertexPath, const GLchar* fragPath);	// Constructor
	void Use();
};