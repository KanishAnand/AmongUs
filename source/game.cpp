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
#include "shader_src/shader.h"

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
    ResourceManager::LoadShader("../source/shaders/color_shader/vertex_shader.vert", "../source/shaders/color_shader/fragment_shader.frag", nullptr, "maze");
    ResourceManager::LoadShader("../source/shaders/texture_shader/vertex_shader.vert", "../source/shaders/texture_shader/fragment_shader.frag", nullptr, "player");
    ResourceManager::LoadShader("../source/shaders/texture_shader/vertex_shader.vert", "../source/shaders/texture_shader/fragment_shader.frag", nullptr, "imposter");
    ResourceManager::LoadShader("../source/HUD/shaders/vertex_shader.vert", "../source/HUD/shaders/fragment_shader.frag", nullptr, "hud");
    ResourceManager::LoadShader("../source/shaders/texture_shader/vertex_shader.vert", "../source/shaders/texture_shader/fragment_shader.frag", nullptr, "button");
    ResourceManager::LoadShader("../source/shaders/color_shader/vertex_shader.vert", "../source/shaders/color_shader/fragment_shader.frag", nullptr, "powerenabler");
    ResourceManager::LoadShader("../source/shaders/texture_shader/vertex_shader.vert", "../source/shaders/texture_shader/fragment_shader.frag", nullptr, "powerups");
    ResourceManager::LoadShader("../source/shaders/texture_shader/vertex_shader.vert", "../source/shaders/texture_shader/fragment_shader.frag", nullptr, "obstacle");
    ResourceManager::LoadShader("../source/shaders/texture_shader/vertex_shader.vert", "../source/shaders/texture_shader/fragment_shader.frag", nullptr, "endcell");

    ResourceManager::GetShader("player").Use().SetInteger("image", 0);
    ResourceManager::GetShader("imposter").Use().SetInteger("image", 0);
    ResourceManager::GetShader("hud").Use().SetInteger("image", 0);
    ResourceManager::GetShader("button").Use().SetInteger("image", 0);
    ResourceManager::GetShader("powerups").Use().SetInteger("image", 0);
    ResourceManager::GetShader("obstacle").Use().SetInteger("image", 0);
    ResourceManager::GetShader("endcell").Use().SetInteger("image", 0);

    // load text renderer
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../source/fonts/antonia_bold.ttf", 26);

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
    ResourceManager::LoadTexture("../source/HUD/texture/texture.jpg", true, "hud");
    ResourceManager::LoadTexture("../source/Player/sprites/0.png", true, "player0");
    ResourceManager::LoadTexture("../source/Player/sprites/1.png", true, "player1");
    ResourceManager::LoadTexture("../source/Player/sprites/2.png", true, "player2");
    ResourceManager::LoadTexture("../source/Player/sprites/3.png", true, "player3");
    ResourceManager::LoadTexture("../source/Imposter/sprites/0.png", true, "imposter0");
    ResourceManager::LoadTexture("../source/Imposter/sprites/1.png", true, "imposter1");
    ResourceManager::LoadTexture("../source/Imposter/sprites/2.png", true, "imposter2");
    ResourceManager::LoadTexture("../source/Imposter/sprites/3.png", true, "imposter3");

    ResourceManager::LoadTexture("../source/Button/texture/button.png", true, "button");

    ResourceManager::LoadTexture("../source/PowerUp/texture/coin.jpg", true, "powerups");

    ResourceManager::LoadTexture("../source/Obstacle/texture/bomb.png", true, "obstacles");

    ResourceManager::LoadTexture("../source/EndCell/texture/end.png", false, "endcell");
}

void Game::Update(float dt) {
}

void Game::ProcessInput(float dt) {
    if (this->Keys[GLFW_KEY_UP]) {
        if (this->globalCount % 10 == 1) {
            this->countPlayer++;
        }
        Player->move(UP, Maze->walls);
        this->KeysProcessed[GLFW_KEY_UP] = true;
    }
    if (this->Keys[GLFW_KEY_DOWN]) {
        if (this->globalCount % 10 == 1) {
            this->countPlayer++;
        }

        Player->move(DOWN, Maze->walls);
        this->KeysProcessed[GLFW_KEY_DOWN] = true;
    }
    if (this->Keys[GLFW_KEY_LEFT]) {
        if (this->globalCount % 10 == 1) {
            this->countPlayer++;
        }

        Player->move(LEFT, Maze->walls);
        this->KeysProcessed[GLFW_KEY_LEFT] = true;
    }
    if (this->Keys[GLFW_KEY_RIGHT]) {
        if (this->globalCount % 10 == 1) {
            this->countPlayer++;
        }

        Player->move(RIGHT, Maze->walls);
        this->KeysProcessed[GLFW_KEY_RIGHT] = true;
    }
    if (this->Keys[GLFW_KEY_O] && !this->KeysProcessed[GLFW_KEY_O]) {
        if (this->light == ON) {
            this->light = OFF;
        } else {
            this->light = ON;
        }
        this->KeysProcessed[GLFW_KEY_O] = true;
    }
}

void Game::set_lighting(Shader &shader) {
    shader.Use();
    shader.SetVector3f("lightColor", glm::vec3(1.0, 1.0, 1.0));
    glm::vec4 lightCoord = vec4(Player->current_coordinates.first, Player->current_coordinates.second, 0.0, 1.0);
    lightCoord = Maze->Model * lightCoord;
    shader.SetVector3f("lightPos", vec3(lightCoord));
    if (this->light == ON) {
        shader.SetInteger("light", 1);
    } else {
        shader.SetInteger("light", 0);
    }
}

void Game::Render() {
    this->globalCount++;

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
        return;
    }

    if (this->State == GAME_ACTIVE && this->playerHealth == 0) {
        this->State = GAME_LOOSE;
        return;
    }

    if (this->State == GAME_ACTIVE && Imposter->active && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, Imposter->current_coordinates, Imposter->IMPOSTER_HEIGHT, Imposter->IMPOSTER_WIDTH)) {
        this->playerHealth -= Imposter->healthDec;
    }

    if (this->State == GAME_ACTIVE && Button->active && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, Button->coordinates, Button->OBJECT_HEIGHT, Button->OBJECT_WIDTH)) {
        this->tasksCompleted++;
        Button->active = false;
        Imposter->active = false;
    }

    if (this->State == GAME_ACTIVE && this->check_collision(Player->current_coordinates, Player->PLAYER_HEIGTH, Player->PLAYER_WIDTH, EndCell->coordinates, EndCell->OBJECT_HEIGHT, EndCell->OBJECT_WIDTH)) {
        if (this->tasksCompleted == 2) {
            this->State = GAME_WIN;
        }
    }

    if (this->light == OFF && this->globalCount % 100 == 0) {
        this->playerHealth++;
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

    set_lighting(Maze->shader);
    Maze->DrawMaze();

    int no = this->countPlayer % 4;
    string st = "player" + to_string(no);
    auto texture_player = ResourceManager::GetTexture(&st[0]);
    set_lighting(Player->shader);
    Player->DrawPlayer(texture_player);

    if (Imposter->active) {
        int no2 = this->countImposter % 4;
        string st2 = "imposter" + to_string(no2);
        auto texture_imposter = ResourceManager::GetTexture(&st2[0]);
        set_lighting(Imposter->shader);
        Imposter->DrawImposter(texture_imposter);

        Imposter->move(Player->current_coordinates, Maze->ROOM_LENGTH, Maze->MAZE_WIDTH, Maze->nearestCell);
        if (this->globalCount % 10 == 0) {
            this->countImposter++;
        }
    }

    // Heads Up Display
    auto texture_hud = ResourceManager::GetTexture("hud");
    HUD->DrawHUD(texture_hud);
    float start = 0, offset = 10.0, x = 30.0;
    string healthStatus = "Health: " + to_string(this->playerHealth);
    Text->RenderText(healthStatus, x, start + 2 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
    string taskStatus = "Tasks Completed: " + to_string(this->tasksCompleted) + "/" + to_string(this->totalTasks);
    Text->RenderText(&taskStatus[0], x, start + 6 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
    string lightStatus = "Lights: ";
    if (this->light == ON) {
        lightStatus += "ON";
    } else {
        lightStatus += "OFF";
    }
    Text->RenderText(lightStatus, x, start + 10 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));
    string timeStatus = "Time Left: " + to_string(this->totalTime);
    Text->RenderText(timeStatus, x, start + 14 * offset, 1.0f, glm::vec3(0.0f, 0.5f, 0.0f));

    //vapourizer button
    if (Button->active) {
        auto texture_button = ResourceManager::GetTexture("button");
        set_lighting(Button->shader);
        Button->DrawButton(texture_button);
    }

    if (PowerEnabler->active) {
        set_lighting(PowerEnabler->shader);
        PowerEnabler->DrawPowerEnabler();
    }

    for (int i = 0; i < Maze->totalPowerups; i++) {
        if (PowerUps[i]->active) {
            auto texture_powerups = ResourceManager::GetTexture("powerups");
            set_lighting(PowerUps[i]->shader);
            PowerUps[i]->DrawPowerUp(texture_powerups);
        }
    }

    for (int i = 0; i < Maze->totalObstacles; i++) {
        if (Obstacles[i]->active) {
            auto texture_obstacle = ResourceManager::GetTexture("obstacles");
            set_lighting(Obstacles[i]->shader);
            Obstacles[i]->DrawObstacle(texture_obstacle);
        }
    }

    auto texture_endcell = ResourceManager::GetTexture("endcell");
    set_lighting(EndCell->shader);
    if (this->tasksCompleted == 2) {
        EndCell->DrawEndCell(texture_endcell);
    }
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