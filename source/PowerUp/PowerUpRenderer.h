#ifndef POWERUPRENDERER_H
#define POWERUPRENDERER_H

#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class PowerUpRenderer {
   public:
    bool active = false;
    int healthInc = 20;
    float OBJECT_HEIGHT = 0.08;
    float OBJECT_WIDTH = 0.08;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> coordinates;

    // Constructor
    PowerUpRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> PowerUp_coord);
    // Destructor
    ~PowerUpRenderer();

    void DrawPowerUp(Texture2D &texture);

   private:
    // Render state
    Shader shader;
    unsigned int VAO;
    void initRenderData();
};
#endif