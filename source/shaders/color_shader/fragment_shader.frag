#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 ourColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform int light;

void main() {
    FragColor = vec4(ourColor, 1.0f);
    float distance = length(FragPos - lightPos);

    if(light == 0){
        FragColor = vec4(ourColor * lightColor * (0.2/distance), 1.0);
    }
    else{
        FragColor = vec4(ourColor, 1.0);
    }
}