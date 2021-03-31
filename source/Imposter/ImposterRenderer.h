#ifndef IMPOSTERRENDERER_H
#define IMPOSTERRENDERER_H

#include "objectCoordinates.h"
#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class ImposterRenderer {
   public:
    bool flip = 1, active = true;
    float IMPOSTER_SPEED = 0.005;
    float IMPOSTER_HEIGHT = 0.1;
    float IMPOSTER_WIDTH = 0.1;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> start_coordinates, current_coordinates;

    Direction current_move = NONE;

    // Constructor
    ImposterRenderer(Shader &shader, pair<float, float> source, glm::mat4 maze_modelmat);
    // Destructor
    ~ImposterRenderer();

    void DrawImposter(Texture2D &texture);
    void move(pair<float, float> playe_coord, float room_length, int MAZE_WIDTH, vector<vector<Direction>> &nearestCell);

   private:
    // Render state
    Shader shader;
    unsigned int VAO;
    void initRenderData();
    bool check_collision(vector<ObjectCoordinates> &walls);
};
#endif