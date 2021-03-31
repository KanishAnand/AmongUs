#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;

void main()
{
    color = 0.8*texture(sprite, TexCoords);
}