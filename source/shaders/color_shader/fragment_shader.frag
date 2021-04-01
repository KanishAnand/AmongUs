#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 ourColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightCutOff;

void main() {
    FragColor = vec4(ourColor, 1.0f);
    float distance = length(FragPos - lightPos);

    if(distance < lightCutOff)
        FragColor = vec4(ourColor * lightColor, 1.0);
    else
        FragColor = vec4(0.3 * ourColor * lightColor, 1.0);
}