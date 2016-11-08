#version 330 core

// Default vertex shader who draw vertex position to the screen

layout(location = 0) in vec3 position;

void main()
{
  gl_Position =  vec4(position,1.0f);  
}
