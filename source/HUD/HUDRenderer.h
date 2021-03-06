#ifndef HUD_H
#define HUD_H

#include "shader_src/shader.h"
#include "template.h"
#include "texture_src/texture.h"

class HUDRenderer {
   public:
    Shader shader;
    // Constructor
    HUDRenderer(Shader &shader, int height, int width);
    // Destructor
    ~HUDRenderer();

    void DrawHUD(Texture2D &texture);

   private:
    // Render state
    unsigned int VAO;
    void initRenderData(int height, int width);
};
#endif