#include "HUDRenderer.h"

HUDRenderer::HUDRenderer(Shader &shader, int height, int width) {
    this->shader = shader;
    this->initRenderData(height, width);
}

HUDRenderer::~HUDRenderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

void HUDRenderer::initRenderData(int height, int width) {
    float w = 0.43;
    float h = 0.5;
    unsigned int VBO;

    float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        w, 0.0f, 1.0f, 0.0f,
        w, h, 1.0f, 1.0f,
        0.0f, h, 0.0f, 1.0f};

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void HUDRenderer::DrawHUD(Texture2D &texture) {
    this->shader.Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.64f, 0.0f));
    this->shader.SetMatrix4("model", model);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}