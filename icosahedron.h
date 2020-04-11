#pragma once

#include "shader.h"

unsigned int make_icosahedron();
void draw_icosahedron(Shader shader, glm::mat4 transform, unsigned int icosahedron_va);
