#include "MazeRenderer.h"

#include "template.h"

MazeRenderer::MazeRenderer(Shader &shader) {
    this->Model = glm::translate(this->Model, glm::vec3(-0.75f, -0.55f, 0.0f));
    this->shader = shader;
    this->maze.resize(this->MAZE_HEIGHT, vector<MazeCell>(this->MAZE_WIDTH));

    int mx = this->MAZE_HEIGHT * this->MAZE_WIDTH, INF = 1e9;
    this->dist.resize(mx, vector<int>(mx, INF));
    this->nearestCell.resize(mx, vector<Direction>(mx, NONE));

    for (int i = 0; i < mx; i++) {
        this->dist[i][i] = 0;
    }

    this->initRenderData();
}

MazeRenderer::~MazeRenderer() {
    for (int i = 0; i < idx; i++) {
        glDeleteVertexArrays(1, &this->VAOS[i]);
    }
}

void MazeRenderer::initRenderData() {
    this->opencloseGates();

    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            pair<float, float> bottomLeft = {j * ROOM_LENGTH, i * ROOM_LENGTH};
            pair<float, float> bottomRight = {(j + 1) * ROOM_LENGTH, i * ROOM_LENGTH};
            pair<float, float> topLeft = {j * ROOM_LENGTH, (i + 1) * ROOM_LENGTH};
            pair<float, float> topRight = {(j + 1) * ROOM_LENGTH, (i + 1) * ROOM_LENGTH};

            if (maze[i][j].bottomGate) {
                this->initializeBuffers(idx, bottomLeft, bottomRight);
                this->walls.push_back(ObjectCoordinates(bottomLeft, bottomRight));
                this->idx++;
            }
            if (maze[i][j].topGate) {
                this->initializeBuffers(idx, topLeft, topRight);
                this->walls.push_back(ObjectCoordinates(topLeft, topRight));
                this->idx++;
            }
            if (maze[i][j].leftGate) {
                this->initializeBuffers(idx, bottomLeft, topLeft);
                this->walls.push_back(ObjectCoordinates(bottomLeft, topLeft));
                this->idx++;
            }
            if (maze[i][j].rightGate) {
                this->initializeBuffers(idx, bottomRight, topRight);
                this->walls.push_back(ObjectCoordinates(bottomRight, topRight));
                this->idx++;
            }
        }
    }

    this->shortestPath();
}

void MazeRenderer::shortestPath() {
    int mx = this->MAZE_HEIGHT * this->MAZE_WIDTH;

    for (int connect = 0; connect < mx; connect++) {
        for (int src = 0; src < mx; src++) {
            for (int des = 0; des < mx; des++) {
                if (this->dist[src][des] > this->dist[src][connect] + this->dist[connect][des]) {
                    this->dist[src][des] = this->dist[src][connect] + this->dist[connect][des];
                    this->nearestCell[src][des] = this->nearestCell[src][connect];
                }
            }
        }
    }
}

void MazeRenderer::initializeBuffers(int idx, pair<float, float> first, pair<float, float> second) {
    unsigned int VBO;
    this->VAOS.push_back(0);

    float vertices[] = {
        first.first, first.second, 0.0f, 0.0f, 0.0f, 0.5f,
        second.first, second.second, 0.0f, 0.0f, 0.0f, 0.5f};

    glGenVertexArrays(1, &this->VAOS[idx]);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAOS[idx]);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MazeRenderer::DrawMaze() {
    glUseProgram(this->shader.ID);

    this->shader.SetMatrix4("Model", this->Model);

    for (int i = 0; i < idx; i++) {
        glBindVertexArray(this->VAOS[i]);
        glDrawArrays(GL_LINES, 0, 2);
    }
    glBindVertexArray(0);
}

void MazeRenderer::opencloseGates() {
    int dis = -1;
    int row = 0, col = 0;

    stack<pair<int, int>> st;
    vector<vector<int>> vis(MAZE_HEIGHT, vector<int>(MAZE_WIDTH, 0));

    st.push({0, 0});
    vis[0][0] = 1;

    while (!st.empty()) {
        pair<int, int> pr = st.top();
        row = pr.first, col = pr.second;
        st.pop();

        if (row + col > dis) {
            dis = row + col;
            this->farthest_coord = {float(col) * this->ROOM_LENGTH, float(row) * this->ROOM_LENGTH};
        }

        bool all_visited = 1;

        if (col + 1 < MAZE_WIDTH && vis[row][col + 1] == 0) {
            all_visited = 0;
        }
        if (col - 1 >= 0 && vis[row][col - 1] == 0) {
            all_visited = 0;
        }
        if (row + 1 < MAZE_HEIGHT && vis[row + 1][col] == 0) {
            all_visited = 0;
        }
        if (row - 1 >= 0 && vis[row - 1][col] == 0) {
            all_visited = 0;
        }

        if (all_visited) {
            continue;
        }

        st.push({row, col});

        while (1) {
            int move = rand() % 4;

            if (move == 0 && col + 1 < MAZE_WIDTH && vis[row][col + 1] == 0) {
                st.push({row, col + 1});
                vis[row][col + 1] = 1;

                int src = row * this->MAZE_WIDTH + col;
                int des = row * this->MAZE_WIDTH + col + 1;
                this->dist[src][des] = 1;
                this->dist[des][src] = 1;
                this->nearestCell[src][des] = RIGHT;
                this->nearestCell[des][src] = LEFT;

                openrightGate(row, col);
                break;
            }
            if (move == 1 && col - 1 >= 0 && vis[row][col - 1] == 0) {
                st.push({row, col - 1});
                vis[row][col - 1] = 1;

                int src = row * this->MAZE_WIDTH + col;
                int des = row * this->MAZE_WIDTH + col - 1;
                this->dist[src][des] = 1;
                this->dist[des][src] = 1;
                this->nearestCell[src][des] = LEFT;
                this->nearestCell[des][src] = RIGHT;

                openleftGate(row, col);
                break;
            }
            if (move == 2 && row + 1 < MAZE_HEIGHT && vis[row + 1][col] == 0) {
                st.push({row + 1, col});
                vis[row + 1][col] = 1;

                int src = row * this->MAZE_WIDTH + col;
                int des = (row + 1) * this->MAZE_WIDTH + col;
                this->dist[src][des] = 1;
                this->dist[des][src] = 1;
                this->nearestCell[src][des] = UP;
                this->nearestCell[des][src] = DOWN;

                opentopGate(row, col);
                break;
            }
            if (move == 3 && row - 1 >= 0 && vis[row - 1][col] == 0) {
                st.push({row - 1, col});
                vis[row - 1][col] = 1;

                int src = row * this->MAZE_WIDTH + col;
                int des = (row - 1) * this->MAZE_WIDTH + col;
                this->dist[src][des] = 1;
                this->dist[des][src] = 1;
                this->nearestCell[src][des] = DOWN;
                this->nearestCell[des][src] = UP;

                openbottomGate(row, col);
                break;
            }
        }
    }

    vector<pair<int, int>> reachables;
    for (row = 0; row < MAZE_HEIGHT; row++) {
        for (col = 0; col < MAZE_WIDTH; col++) {
            if (row + col != dis && row + col != 0 && vis[row][col]) {
                reachables.push_back({row, col});
            }
        }
    }

    random_shuffle(reachables.begin(), reachables.end());

    this->button_coord = {float(reachables[0].second) * this->ROOM_LENGTH, float(reachables[0].first) * this->ROOM_LENGTH};
    this->imposter_coord = {float(reachables[1].second) * this->ROOM_LENGTH, float(reachables[1].first) * this->ROOM_LENGTH};
    this->powerenabler_coord = {float(reachables[2].second) * this->ROOM_LENGTH, float(reachables[2].first) * this->ROOM_LENGTH};
}

void MazeRenderer::openrightGate(int &row, int &col) {
    maze[row][col].rightGate = 0;
    col++;  // go right
    maze[row][col].leftGate = 0;
}

void MazeRenderer::openleftGate(int &row, int &col) {
    maze[row][col].leftGate = 0;
    col--;  // go left
    maze[row][col].rightGate = 0;
}

void MazeRenderer::opentopGate(int &row, int &col) {
    maze[row][col].topGate = 0;
    row++;  //go up
    maze[row][col].bottomGate = 0;
}

void MazeRenderer::openbottomGate(int &row, int &col) {
    maze[row][col].bottomGate = 0;
    row--;  //go down
    maze[row][col].topGate = 0;
}