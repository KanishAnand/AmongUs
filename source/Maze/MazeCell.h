#ifndef MAZE_CELL_H
#define MAZE_CELL_H

class MazeCell {
   public:
    bool leftGate = 1, rightGate = 1, topGate = 1, bottomGate = 1;

    // constructor
    MazeCell() {
    }
};
#endif