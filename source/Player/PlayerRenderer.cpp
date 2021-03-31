#include "PlayerRenderer.h"

PlayerRenderer::PlayerRenderer(Shader &shader, glm::mat4 maze_modelmat) {
    this->start_coordinates = {0.025, 0.03};
    this->current_coordinates = start_coordinates;
    this->Model = maze_modelmat;
    this->shader = shader;
    this->initRenderData();
}

PlayerRenderer::~PlayerRenderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

void PlayerRenderer::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        start_coordinates.first, start_coordinates.second, 0.0f, 1.0f,
        start_coordinates.first + PLAYER_WIDTH, start_coordinates.second, 1.0f, 1.0f,
        start_coordinates.first + PLAYER_WIDTH, start_coordinates.second + PLAYER_HEIGTH, 1.0f, 0.0f,
        start_coordinates.first, start_coordinates.second + PLAYER_HEIGTH, 0.0f, 0.0f};

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

void PlayerRenderer::DrawPlayer(Texture2D &texture) {
    this->shader.Use();

    glm::mat4 model = this->Model;
    glm::mat4 rot = glm::mat4(1.0f);

    if (this->flip) {
        // rot = glm::rotate(rot, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    this->shader.SetMatrix4("model", model * rot);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

bool PlayerRenderer::check_collision(vector<ObjectCoordinates> &walls) {
    for (auto wall : walls) {
        float x_dis = std::min(wall.topRight.first, this->current_coordinates.first + PLAYER_WIDTH) -
                      std::max(wall.bottomLeft.first, this->current_coordinates.first);
        float y_dis = std::min(wall.topRight.second, this->current_coordinates.second + PLAYER_HEIGTH) -
                      std::max(wall.bottomLeft.second, this->current_coordinates.second);
        if (x_dis >= 0 && y_dis >= 0) {
            return 1;
        }
    }
    return 0;
}

void PlayerRenderer::move(Direction direction, vector<ObjectCoordinates> &walls) {
    if (direction == UP) {
        this->current_coordinates.second += PLAYER_SPEED;
        if (this->check_collision(walls)) {
            this->current_coordinates.second -= PLAYER_SPEED;
        } else {
            this->Model = glm::translate(this->Model, glm::vec3(0.0f, PLAYER_SPEED, 0.0f));
        }
    }
    if (direction == DOWN) {
        this->current_coordinates.second -= PLAYER_SPEED;
        if (this->check_collision(walls)) {
            this->current_coordinates.second += PLAYER_SPEED;
        } else {
            this->Model = glm::translate(this->Model, glm::vec3(0.0f, -PLAYER_SPEED, 0.0f));
        }
    }
    if (direction == LEFT) {
        this->current_coordinates.first -= PLAYER_SPEED;
        if (this->check_collision(walls)) {
            this->current_coordinates.first += PLAYER_SPEED;
        } else {
            this->Model = glm::translate(this->Model, glm::vec3(-PLAYER_SPEED, 0.0f, 0.0f));
        }

        this->flip = 1;
    }
    if (direction == RIGHT) {
        this->current_coordinates.first += PLAYER_SPEED;
        if (this->check_collision(walls)) {
            this->current_coordinates.first -= PLAYER_SPEED;
        } else {
            this->Model = glm::translate(this->Model, glm::vec3(PLAYER_SPEED, 0.0f, 0.0f));
        }

        this->flip = 0;
    }
}