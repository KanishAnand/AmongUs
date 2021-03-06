#ifndef OBSTACLERENDERER_H
#define OBSTACLERENDERER_H

#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class ObstacleRenderer {
   public:
    bool active = false;
    int healthDec = 10;
    Shader shader;
    float OBJECT_HEIGHT = 0.08;
    float OBJECT_WIDTH = 0.08;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> coordinates;

    // Constructor
    ObstacleRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> Obstacle_coord);
    // Destructor
    ~ObstacleRenderer();

    void DrawObstacle(Texture2D &texture);

   private:
    // Render state
    unsigned int VAO;
    void initRenderData();
};
#endif