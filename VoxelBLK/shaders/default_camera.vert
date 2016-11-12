#version 330 core

// Default vertex shader who draw vertex relative to the camera

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 v_color;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
  gl_Position =  projection * view * model * vec4(position,1.0f);
  v_color = color;
}
