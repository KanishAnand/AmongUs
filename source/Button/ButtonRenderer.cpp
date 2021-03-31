#include "ButtonRenderer.h"

ButtonRenderer::ButtonRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> button_coord) {
    this->Model = maze_modelmat;
    this->shader = shader;
    this->coordinates = {button_coord.first + 0.025, button_coord.second + 0.03};
    this->initRenderData();
}

ButtonRenderer::~ButtonRenderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

void ButtonRenderer::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;

    float vertices[] = {
        this->coordinates.first, this->coordinates.second, 0.0f, 1.0f,
        this->coordinates.first + OBJECT_WIDTH, this->coordinates.second, 1.0f, 1.0f,
        this->coordinates.first + OBJECT_WIDTH, this->coordinates.second + OBJECT_HEIGHT, 1.0f, 0.0f,
        this->coordinates.first, this->coordinates.second + OBJECT_HEIGHT, 0.0f, 0.0f};

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

void ButtonRenderer::DrawButton(Texture2D &texture) {
    this->shader.Use();

    glm::mat4 model = this->Model;

    this->shader.SetMatrix4("model", model);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

// bool ButtonRenderer::check_collision(vector<ObjectCoordinates> &walls) {
//     for (auto wall : walls) {
//         float x_dis = std::min(wall.topRight.first, this->current_coordinates.first + PLAYER_WIDTH) -
//                       std::max(wall.bottomLeft.first, this->current_coordinates.first);
//         float y_dis = std::min(wall.topRight.second, this->current_coordinates.second + PLAYER_HEIGTH) -
//                       std::max(wall.bottomLeft.second, this->current_coordinates.second);
//         if (x_dis >= 0 && y_dis >= 0) {
//             return 1;
//         }
//     }
//     return 0;
// }