#version 330 core

layout (binding=0) uniform sampler2DArray textureArray;

in vec2 f_texCoords;
in float f_texIndex;
out vec4 color;

void main()
{  
  color = texture(textureArray, vec3(f_texCoords.x,f_texCoords.y, f_texIndex));
}
