/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include "template.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_WIN,
    GAME_LOOSE
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game {
   public:
    // game state
    int countPlayer = 0, countImposter = 0, tasksCompleted = 0, totalTasks = 2, totalTime = 50, playerHealth = 100;
    GameState State;
    bool Keys[1024];
    bool KeysProcessed[1024];
    unsigned int Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    // reset
    void ResetPlayer();
    // powerups
   private:
    bool check_collision(pair<float, float> &coord1, float h1, float w1, pair<float, float> &coord2, float h2, float w2);
};

#endif