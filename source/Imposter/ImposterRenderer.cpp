#include "ImposterRenderer.h"

ImposterRenderer::ImposterRenderer(Shader &shader, pair<float, float> source, glm::mat4 maze_modelmat) {
    this->start_coordinates = {0.0f, 0.0f};
    this->current_coordinates = {source.first, source.second};
    this->Model = maze_modelmat;
    this->Model = glm::translate(this->Model, glm::vec3(current_coordinates.first, current_coordinates.second, 0.0f));
    this->shader = shader;
    this->initRenderData();
}

ImposterRenderer::~ImposterRenderer() {
    glDeleteVertexArrays(1, &this->VAO);
}

void ImposterRenderer::initRenderData() {
    // configure VAO / VBO unsigned int VBO;
    unsigned int VBO;

    float vertices[] = {
        start_coordinates.first, start_coordinates.second, 0.0f, 1.0f,
        start_coordinates.first + IMPOSTER_WIDTH, start_coordinates.second, 1.0f, 1.0f,
        start_coordinates.first + IMPOSTER_WIDTH, start_coordinates.second + IMPOSTER_HEIGHT, 1.0f, 0.0f,
        start_coordinates.first, start_coordinates.second + IMPOSTER_HEIGHT, 0.0f, 0.0f};

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

void ImposterRenderer::DrawImposter(Texture2D &texture) {
    this->shader.Use();

    glm::mat4 model = this->Model;

    if (this->flip) {
        model = glm::translate(model, glm::vec3(0.5f * this->IMPOSTER_WIDTH, 0.5f * this->IMPOSTER_HEIGHT, 0.0f));    // move origin of rotation to center of quad
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));                                // then rotate
        model = glm::translate(model, glm::vec3(-0.5f * this->IMPOSTER_WIDTH, -0.5f * this->IMPOSTER_HEIGHT, 0.0f));  // move origin back
    }

    this->shader.SetMatrix4("model", model);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

bool ImposterRenderer::check_collision(vector<ObjectCoordinates> &walls) {
    for (auto wall : walls) {
        float x_dis = std::min(wall.topRight.first, this->current_coordinates.first + IMPOSTER_WIDTH) -
                      std::max(wall.bottomLeft.first, this->current_coordinates.first);
        float y_dis = std::min(wall.topRight.second, this->current_coordinates.second + IMPOSTER_HEIGHT) -
                      std::max(wall.bottomLeft.second, this->current_coordinates.second);
        if (x_dis >= 0 && y_dis >= 0) {
            return 1;
        }
    }
    return 0;
}

void ImposterRenderer::move(pair<float, float> player_coord, float room_length, int MAZE_WIDTH, vector<vector<Direction>> &nearestCell) {
    int player_row = int(player_coord.second / room_length);
    int player_col = int(player_coord.first / room_length);

    int imposter_row = int(this->current_coordinates.second / room_length);
    int imposter_col = int(this->current_coordinates.first / room_length);

    Direction move;

    if (imposter_row != int((this->current_coordinates.second + this->IMPOSTER_HEIGHT) / room_length)) {
        move = this->current_move;
    } else if (imposter_col != int((this->current_coordinates.first + this->IMPOSTER_WIDTH) / room_length)) {
        move = this->current_move;
    } else {
        int src = imposter_row * MAZE_WIDTH + imposter_col;
        int des = player_row * MAZE_WIDTH + player_col;
        move = nearestCell[src][des];
    }

    this->current_move = move;

    if (move == UP) {
        this->current_coordinates.second += IMPOSTER_SPEED;
        this->Model = glm::translate(this->Model, glm::vec3(0.0f, IMPOSTER_SPEED, 0.0f));
    } else if (move == DOWN) {
        this->current_coordinates.second -= IMPOSTER_SPEED;
        this->Model = glm::translate(this->Model, glm::vec3(0.0f, -IMPOSTER_SPEED, 0.0f));
    } else if (move == LEFT) {
        this->current_coordinates.first -= IMPOSTER_SPEED;
        this->Model = glm::translate(this->Model, glm::vec3(-IMPOSTER_SPEED, 0.0f, 0.0f));
        this->flip = 1;
    } else if (move == RIGHT) {
        this->current_coordinates.first += IMPOSTER_SPEED;
        this->Model = glm::translate(this->Model, glm::vec3(IMPOSTER_SPEED, 0.0f, 0.0f));
        this->flip = 0;
    }
}
