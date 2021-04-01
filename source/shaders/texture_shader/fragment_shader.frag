#version 330 core
in vec2 TexCoords;
in vec3 FragPos;
out vec4 color;

uniform sampler2D sprite;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform int light;

void main()
{
    float distance = length(FragPos - lightPos);

    if(light == 0){
        color = vec4(lightColor*(0.2/distance), 1.0) * texture(sprite, TexCoords);
    }
    else{
        color = texture(sprite, TexCoords);
    }
}