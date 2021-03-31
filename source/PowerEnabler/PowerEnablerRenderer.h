#ifndef POWERENABLERRENDERER_H
#define POWERENABLERRENDERER_H

#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class PowerEnablerRenderer {
   public:
    bool active = true;
    float OBJECT_HEIGHT = 0.1;
    float OBJECT_WIDTH = 0.1;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> coordinates;

    // Constructor
    PowerEnablerRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> PowerEnabler_coord);
    // Destructor
    ~PowerEnablerRenderer();

    void DrawPowerEnabler();

   private:
    // Render state
    Shader shader;
    unsigned int VAO;
    void initRenderData();
};
#endif