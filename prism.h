#pragma once

#include "shader.h"

unsigned int make_prism();
void draw_prism(Shader shader, glm::mat4 transform, unsigned int cube_va);
