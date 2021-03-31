#include "game.h"

#include "Button/ButtonRenderer.h"
#include "EndCell/EndCellRenderer.h"
#include "HUD/HUDRenderer.h"
#include "Imposter/ImposterRenderer.h"
#include "Maze/MazeRenderer.h"
#include "Obstacle/ObstacleRenderer.h"
#include "Player/PlayerRenderer.h"
#include "PowerEnabler/PowerEnablerRenderer.h"
#include "PowerUp/PowerUpRenderer.h"
#include "TextRenderer.h"
#include "resource_manager.h"

PlayerRenderer *Player;
ImposterRenderer *Imposter;
MazeRenderer *Maze;
TextRenderer *Text;
HUDRenderer *HUD;
ButtonRenderer *Button;
PowerEnablerRenderer *PowerEnabler;
PowerUpRenderer *PowerUps[3];
ObstacleRenderer *Obstacles[2];
EndCellRenderer *EndCell;
TextRenderer *TextFinal;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {
}

Game::~Game() {
    // delete Renderer;
}

void Game::Init() {
    // load shaders
    ResourceManager::LoadShader("../source/Maze/shaders/vertex_shader.vert", "../source/Maze/shaders/fragment_shader.frag", nullptr, "maze");
    ResourceManager::LoadShader("../source/Player/shaders/vertex_shader.vert", "../source/Player/shaders/fragment_shader.frag", nullptr, "player");
    ResourceManager::LoadShader("../source/Imposter/shaders/vertex_shader.vert", "../source/Imposter/shaders/fragment_shader.frag", nullptr, "imposter");
    ResourceManager::LoadShader("../source/HUD/shaders/vertex_shader.vert", "../source/HUD/shaders/fragment_shader.frag", nullptr, "hud");
    ResourceManager::LoadShader("../source/Button/shaders/vertex_shader.vert", "../source/Button/shaders/fragment_shader.frag", nullptr, "button");
    ResourceManager::LoadShader("../source/PowerEnabler/shaders/vertex_shader.vert", "../source/PowerEnabler/shaders/fragment_shader.frag", nullptr, "powerenabler");
    ResourceManager::LoadShader("../source/PowerUp/shaders/vertex_shader.vert", "../source/PowerUp/shaders/fragment_shader.frag", nullptr, "powerups");
    ResourceManager::LoadShader("../source/Obstacle/shaders/vertex_shader.vert", "../source/Obstacle/shaders/fragment_shader.frag", nullptr, "obstacle");
    ResourceManager::LoadShader("../source/EndCell/shaders/vertex_shader.vert", "../source/EndCell/shaders/fragment_shader.frag", nullptr, "endcell");

    ResourceManager::GetShader("player").Use().SetInteger("image", 0);
    ResourceManager::GetShader("imposter").Use().SetInteger("image", 0);
    ResourceManager::GetShader("hud").Use().SetInteger("image", 0);
    ResourceManager::GetShader("button").Use().SetInteger("image", 0);
    ResourceManager::GetShader("powerups").Use().SetInteger("image", 0);
    ResourceManager::GetShader("obstacle").Use().SetInteger("image", 0);
    ResourceManager::GetShader("endcell").Use().SetInteger("image", 0);

    // load text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../source/fonts/antonia_bold.ttf", 28);

    TextFinal = new TextRenderer(this->Width, this->Height);
    TextFinal->Load("../source/fonts/antonia_bold.ttf", 50);

    // set render-specific controls
    auto shader_maze = ResourceManager::GetShader("maze");
    Maze = new MazeRenderer(shader_maze);

    auto shader_player = ResourceManager::GetShader("player");
    Player = new PlayerRenderer(shader_player, Maze->Model);

    auto shader_imposter = ResourceManager::GetShader("imposter");
    Imposter = new ImposterRenderer(shader_imposter, Maze->imposter_coord, Maze->Model);

    auto shader_hud = ResourceManager::GetShader("hud");
    HUD = new HUDRenderer(shader_hud, this->Height, this->Width);

    auto shader_button = ResourceManager::GetShader("button");
    Button = new ButtonRenderer(shader_button, Maze->Model, Maze->button_coord);

    auto shader_powerenabler = ResourceManager::GetShader("powerenabler");
    PowerEnabler = new PowerEnablerRenderer(shader_powerenabler, Maze->Model, Maze->powerenabler_coord);

    auto shader_powerup = ResourceManager::GetShader("powerups");
    for (int i = 0; i < Maze->totalPowerups; i++) {
        PowerUps[i] = new PowerUpRenderer(shader_powerup, Maze->Model, Maze->powerup_coord[i]);
    }

    auto shader_obstacle = ResourceManager::GetShader("obstacle");
    for (int i = 0; i < Maze->totalObstacles; i++) {
        Obstacles[i] = new ObstacleRenderer(shader_obstacle, Maze->Model, Maze->obstacle_coord[i]);
    }

    auto shader_endcell = ResourceManager::GetShader("endcell");
    EndCell = new EndCellRenderer(shader_endcell, Maze->Model, Maze->farthest_coord);

    //Load texture
    ResourceManager::LoadTexture("../source/HUD/texture/download.jpg", true, "hud");
    ResourceManager::LoadTexture("../source/Player/sprites/0.png", true, "player0");
    ResourceManager::LoadTexture("../source/Player/sprites/1.png", true, "player1");
    ResourceManager::LoadTexture("../source/Player/sprites/2.png", true, "player2");
    ResourceManager::LoadTexture("../source/Player/sprites/3.png", true, "player3");
    ResourceManager::LoadTexture("../source/Player/sprites/4.png", true, "player4");
    ResourceManager::LoadTexture("../source/Player/sprites/5.png", true, "player5");
    ResourceManager::LoadTexture("../source/Player/sprites/6.png", true, "player6");
    ResourceManager::LoadTexture("../source/Player/sprites/7.png", true, "player7");
    ResourceManager::LoadTexture("../source/Player/sprites/8.png", true, "player8");
    ResourceManager::LoadTexture("../source/Player/sprites/9.png", true, "player9");
    ResourceManager::LoadTexture("../source/Player/sprites/10.png", true, "player10");
    ResourceManager::LoadTexture("../source/Player/sprites/11.png", true, "player11");
    ResourceManager::LoadTexture("../source/Player/sprites/0.png", true, "imposter0");
    ResourceManager::LoadTexture("../source/Player/sprites/1.png", true, "imposter1");
    ResourceManager::LoadTexture("../source/Player/sprites/2.png", true, "imposter2");
    ResourceManager::LoadTexture("../source/Player/sprites/3.png", true, "imposter3");
    ResourceManager::LoadTexture("../source/Player/sprites/4.png", true, "imposter4");
    ResourceManager::LoadTexture("../source/Player/sprites/5.png", true, "imposter5");

    ResourceManager::LoadTexture("../source/Button/texture/kill.png", true, "button");

    ResourceManager::LoadTexture("../source/PowerUp/texture/coin.jpg", true, "powerups");

    ResourceManager::LoadTexture("../source/Obstacle/texture/bomb.png", true, "obstacles");

    ResourceManager::LoadTexture("../source/EndCell/texture/end.png", true, "endcell");
}

void Game::Update(float dt) {
}

void Game::ProcessInput(float dt) {
    if (this->Keys[GLFW_KEY_UP]) {
        this->countPlayer++;
        Player->move(UP, Maze->walls);
        this->KeysProcessed[GLFW_KEY_UP] = true;
    }
    if (this->Keys[GLFW_KEY_DOWN]) {
        this->countPlayer++;
        Player->move(DOWN, Maze->walls);
        this->KeysProcessed[GLFW_KEY_DOWN] = true;
    }
    if (this->Keys[GLFW_KEY_LEFT]) {
        this->countPlayer++;
        Player->move(LEFT, Maze->walls);
        this->KeysProcessed[GLFW_KEY_LEFT] = true;
    }
    if (this->Keys[GLFW_KEY_RIGHT]) {
        this->countPlayer++;
        Player->move(RIGHT, Maze->walls);
        this->KeysProcessed[GLFW_KEY_RIGHT] = true;
    }
}

void Game::Render() {
    if (this->State == GAME_LOOSE) {
        float xx = float(this->Width) / 2.5, yy = float(this->Height) / float(2.7);
        TextFinal->RenderText("You Lost :(", xx, yy, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        string healthStatuss = "Your Health: " + to_string(this->playerHealth);
        Text->RenderText(healthStatuss, xx, yy + 100, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        string taskStatuss = "Tasks Completed: " + to_string(this->tasksCompleted) + "/" + to_string(this->totalTasks);
        Text->RenderText(taskStatuss, xx, yy + 150, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        string timeLeft = "Time Left: " + to_string(this->totalTime);
        Text->RenderText(timeLeft, xx, yy + 200, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        return;
    }

    if (this->State == GAME_WIN) {
        float xx = float(this->Width) / 2.5, yy = float(this->Height) / float(2.7);
        TextFinal->RenderText("You Won :)", xx, yy, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        string healthStatuss = "Your Health: " + to_string(this->playerHealth);
        Text->RenderText(healthStatuss, xx, yy + 100, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        string taskStatuss = "Tasks Completed: " + to_string(this->tasksCompleted) + "/" + to_string(this->totalTasks);
        Text->RenderText(taskStatuss, xx, yy + 150, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        string timeLeft = "Time Left: " + to_string(this->totalTime);
        Text->RenderText(timeLeft, xx, yy + 200, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
        return;
        return;
    }

    if (this->State == GAME_ACTIVE && this->totalTime == 0) {
        this->State = GAME_LOOSE;
    }

    if (this->State == GAME_ACTIVE && this->playerHealth == 0) {
        this->State = GAME_LOOSE;
    }

    if (this->State == GAME_ACTIVE && Imposter->active && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, Imposter->current_coordinates, Imposter->IMPOSTER_HEIGHT, Imposter->IMPOSTER_WIDTH)) {
        this->playerHealth -= Imposter->healthDec;
    }

    if (this->State == GAME_ACTIVE && Button->active && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, Button->coordinates, Button->OBJECT_HEIGHT, Button->OBJECT_WIDTH)) {
        // this->State = GAME_VAP;
        this->tasksCompleted++;
        Button->active = false;
        Imposter->active = false;
    }

    if (this->State == GAME_ACTIVE && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, EndCell->coordinates, EndCell->OBJECT_HEIGHT, EndCell->OBJECT_WIDTH)) {
        if (this->tasksCompleted == 2) {
            this->State = GAME_WIN;
        }
    }

    if (this->State == GAME_ACTIVE && PowerEnabler->active && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, PowerEnabler->coordinates, PowerEnabler->OBJECT_HEIGHT, PowerEnabler->OBJECT_WIDTH)) {
        PowerEnabler->active = false;
        for (int i = 0; i < Maze->totalPowerups; i++) {
            PowerUps[i]->active = true;
        }
        for (int i = 0; i < Maze->totalObstacles; i++) {
            Obstacles[i]->active = true;
        }
    }

    int flag = 1;
    for (int i = 0; i < Maze->totalPowerups; i++) {
        if (PowerUps[i]->active && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, PowerUps[i]->coordinates, PowerUps[i]->OBJECT_HEIGHT, PowerUps[i]->OBJECT_WIDTH)) {
            PowerUps[i]->active = false;
            this->playerHealth += PowerUps[i]->healthInc;
            flag = 0;
        }
    }

    if (flag == 0) {
        int tmp = 1;
        for (int i = 0; i < Maze->totalPowerups; i++) {
            if (PowerUps[i]->active) {
                tmp = 0;
            }
        }
        if (tmp) {
            this->tasksCompleted++;
        }
    }

    for (int i = 0; i < Maze->totalObstacles; i++) {
        if (Obstacles[i]->active && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, Obstacles[i]->coordinates, Obstacles[i]->OBJECT_HEIGHT, Obstacles[i]->OBJECT_WIDTH)) {
            Obstacles[i]->active = false;
            this->playerHealth -= Obstacles[i]->healthDec;
        }
    }

    Maze->DrawMaze();

    int no = this->countPlayer % 12;
    string st = "player" + to_string(no);
    auto texture_player = ResourceManager::GetTexture(&st[0]);
    Player->DrawPlayer(texture_player);

    if (Imposter->active) {
        int no2 = this->countImposter % 6;
        string st2 = "imposter" + to_string(no2);
        auto texture_imposter = ResourceManager::GetTexture(&st2[0]);
        Imposter->DrawImposter(texture_imposter);

        Imposter->move(Player->current_coordinates, Maze->ROOM_LENGTH, Maze->MAZE_WIDTH, Maze->nearestCell);
        this->countImposter++;
    }

    // Heads Up Display
    auto texture_hud = ResourceManager::GetTexture("hud");
    HUD->DrawHUD(texture_hud);
    float start = 0, offset = 10.0, x = 20.0;
    string healthStatus = "Health: " + to_string(this->playerHealth);
    Text->RenderText(healthStatus, x, start + 2 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
    string taskStatus = "Tasks Completed: " + to_string(this->tasksCompleted) + "/" + to_string(this->totalTasks);
    Text->RenderText(&taskStatus[0], x, start + 6 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
    Text->RenderText("Lights: On", x, start + 10 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
    string timeStatus = "Time Left: " + to_string(this->totalTime);
    Text->RenderText(timeStatus, x, start + 14 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));

    //vapourizer button
    if (Button->active) {
        auto texture_button = ResourceManager::GetTexture("button");
        Button->DrawButton(texture_button);
    }

    if (PowerEnabler->active) {
        PowerEnabler->DrawPowerEnabler();
    }

    for (int i = 0; i < Maze->totalPowerups; i++) {
        if (PowerUps[i]->active) {
            auto texture_powerups = ResourceManager::GetTexture("powerups");
            PowerUps[i]->DrawPowerUp(texture_powerups);
        }
    }

    for (int i = 0; i < Maze->totalObstacles; i++) {
        if (Obstacles[i]->active) {
            auto texture_obstacle = ResourceManager::GetTexture("obstacles");
            Obstacles[i]->DrawObstacle(texture_obstacle);
        }
    }

    auto texture_endcell = ResourceManager::GetTexture("endcell");
    EndCell->DrawEndCell(texture_endcell);
}

bool Game::check_collision(pair<float, float> &coord1, float h1, float w1, pair<float, float> &coord2, float h2, float w2) {
    float x_dis = std::min(coord2.first + w2, coord1.first + w1) -
                  std::max(coord2.first, coord1.first);
    float y_dis = std::min(coord2.second + h2, coord1.second + h1) -
                  std::max(coord2.second, coord1.second);

    if (x_dis >= 0 && y_dis >= 0) {
        return 1;
    }
    return 0;
}