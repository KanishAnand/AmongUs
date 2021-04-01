# 2-D-Maze-Game

## INSTALLATION INSTRUCTIONS

- Type following commands:
    ```
    sudo apt-get install build-essential
    sudo apt install xorg-dev libglu1-mesa-dev freeglut3-dev mesa-common-dev
    sudo apt-get install libfreetype-dev libfreetype6 libfreetype6-dev
    pkg-config --cflags freetype2
    ```

- Install `cmake`
    ```
    sudo apt install cmake
    ```

- Run following commands:

    ```
    mkdir build
    cd build
    cmake ..
    make
    ./2-D-Maze-Game
    ```

## HOW TO RUN

1. 4 Keys to move the player.
    - W - Top
    - S - Bottom
    - A - Left
    - D - Right

2. 1 Key to toggle light.
    - L - toggles light
