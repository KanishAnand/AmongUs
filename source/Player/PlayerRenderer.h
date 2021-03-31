#ifndef PLAYERRENDERER_H
#define PLAYERRENDERER_H

#include "objectCoordinates.h"
#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class PlayerRenderer {
   public:
    bool flip = 0;
    float PLAYER_SPEED = 0.015;
    float PLAYER_HEIGTH = 0.1;
    float PLAYER_WIDTH = 0.1;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> start_coordinates, current_coordinates;

    // Constructor
    PlayerRenderer(Shader &shader, glm::mat4 maze_modelmat);
    // Destructor
    ~PlayerRenderer();

    void DrawPlayer(Texture2D &texture);
    void move(Direction direction, vector<ObjectCoordinates> &walls);

   private:
    // Render state
    Shader shader;
    unsigned int VAO;
    void initRenderData();
    bool check_collision(vector<ObjectCoordinates> &walls);
};
#endif