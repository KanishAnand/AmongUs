#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec3 FragPos;

uniform mat4 model;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = model * vec4(vertex.xy, 0.0, 1.0);
    FragPos = vec3(model * vec4(vertex.xy, 0.0, 1.0));
}