#version 330 core

// Default vertex shader who draw vertex relative to the camera

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;
layout(location = 2) in float texIndex;

out vec2 f_texCoords;
out int f_texIndex;
out vec3 f_pos;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
  gl_Position =  projection * view * model * vec4(position,1.0f);
  f_texCoords = texCoords;
  f_texIndex = int(texIndex);
  f_pos = position;
}
