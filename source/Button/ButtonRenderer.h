#ifndef BUTTONRENDERER_H
#define BUTTONRENDERER_H

#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class ButtonRenderer {
   public:
    bool active = true;
    float OBJECT_HEIGHT = 0.1;
    float OBJECT_WIDTH = 0.1;
    Shader shader;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> coordinates;

    // Constructor
    ButtonRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> button_coord);
    // Destructor
    ~ButtonRenderer();

    void DrawButton(Texture2D &texture);

   private:
    // Render state
    unsigned int VAO;
    void initRenderData();
};
#endif