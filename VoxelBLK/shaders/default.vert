#version 330 core

// Default vertex shader who draw vertex position to the screen

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 vertexColor;

void main()
{
  gl_Position =  vec4(position,1.0f);
  vertexColor = color;
}
