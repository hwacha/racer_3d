#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Obstacle { // cubes
  float scale;
  glm::vec3 position; // center of the cube
  // no orientation for now
};
