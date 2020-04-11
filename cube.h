#pragma once

#include "shader.h"

unsigned int make_cube();
void draw_cube(Shader shader, glm::mat4 transform, unsigned int cube_va);
