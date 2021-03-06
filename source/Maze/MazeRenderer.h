#ifndef MAZERENDERER_H
#define MAZERENDERER_H

#include "MazeCell.h"
#include "objectCoordinates.h"
#include "shader_src/shader.h"
#include "template.h"

class MazeRenderer {
   public:
    int idx = 0;
    const int MAZE_HEIGHT = 8;
    const int MAZE_WIDTH = 10;
    const float ROOM_LENGTH = 0.15;

    Shader shader;
    int totalPowerups = 3, totalObstacles = 2;

    vector<pair<float, float>> powerup_coord, obstacle_coord;
    pair<float, float> farthest_coord, button_coord, imposter_coord, powerenabler_coord;
    glm::mat4 Model = glm::mat4(1.0f);

    vector<vector<MazeCell>> maze;
    vector<ObjectCoordinates> walls;

    //edges
    vector<vector<int>> dist;
    vector<vector<Direction>> nearestCell;

    // Constructor
    MazeRenderer(Shader &shader);
    // Destructor
    ~MazeRenderer();

    void DrawMaze();

   private:
    // Render state
    vector<unsigned int> VAOS;
    void initRenderData();
    void initializeBuffers(int idx, pair<float, float> first, pair<float, float> second);
    void opencloseGates();
    void openrightGate(int &row, int &col);
    void openleftGate(int &row, int &col);
    void opentopGate(int &row, int &col);
    void openbottomGate(int &row, int &col);
    void shortestPath();
};
#endif