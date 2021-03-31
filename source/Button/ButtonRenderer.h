#ifndef BUTTONRENDERER_H
#define BUTTONRENDERER_H

#include "objectCoordinates.h"
#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class ButtonRenderer {
   public:
    bool active = true;
    float OBJECT_HEIGHT = 0.1;
    float OBJECT_WIDTH = 0.1;
    glm::mat4 Model = glm::mat4(1.0f);
    pair<float, float> coordinates;

    // Constructor
    ButtonRenderer(Shader &shader, glm::mat4 maze_modelmat, pair<float, float> button_coord);
    // Destructor
    ~ButtonRenderer();

    void DrawButton(Texture2D &texture);
    void move(Direction direction, vector<ObjectCoordinates> &walls);

   private:
    // Render state
    Shader shader;
    unsigned int VAO;
    void initRenderData();
    // bool check_collision(vector<ObjectCoordinates> &walls);
};
#endif