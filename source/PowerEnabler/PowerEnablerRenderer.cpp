#include "PowerEnablerRenderer.h"

PowerEnablerRenderer::PowerEnablerRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> powerenabler_coord) {
    this->Model = maze_modelmat;
    this->shader = shader;
    this->coordinates = {powerenabler_coord.first + 0.025, powerenabler_coord.second + 0.03};
    this->initRenderData();
}

PowerEnablerRenderer::~PowerEnablerRenderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

void PowerEnablerRenderer::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;

    float vertices[] = {
        this->coordinates.first, this->coordinates.second, 0.0f, 0.0f, 1.0f, 0.0f,
        this->coordinates.first + OBJECT_WIDTH, this->coordinates.second, 0.0f, 1.0f, 0.0f, 0.0f,
        this->coordinates.first + OBJECT_WIDTH / 2, this->coordinates.second + OBJECT_HEIGHT, 0.0f, 0.0f, 0.0f, 1.0f};

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PowerEnablerRenderer::DrawPowerEnabler() {
    this->shader.Use();

    glm::mat4 model = this->Model;

    this->shader.SetMatrix4("Model", model);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}