#version 330 core

// Default fragment shader who return a pink colored vertex

in vec4 v_color;

out vec4 color;

void main()
{
  color =  v_color;
}
