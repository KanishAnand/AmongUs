#ifndef ENDCELLRENDERER_H
#define ENDCELLRENDERER_H

#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class EndCellRenderer {
   public:
    bool active = false;
    int healthInc = 20;
    Shader shader;
    float OBJECT_HEIGHT = 0.15;
    float OBJECT_WIDTH = 0.15;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> coordinates;

    // Constructor
    EndCellRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> EndCell_coord);
    // Destructor
    ~EndCellRenderer();

    void DrawEndCell(Texture2D &texture);

   private:
    // Render state
    unsigned int VAO;
    void initRenderData();
};
#endif