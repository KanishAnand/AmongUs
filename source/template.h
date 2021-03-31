#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// c++
#include <bits/stdc++.h>

// font library
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace glm;
using namespace std;

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};

#endif
