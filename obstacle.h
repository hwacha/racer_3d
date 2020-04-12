#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Obstacle { // cubes
  // 0 = floor
  // 1 = stop
  // 2 = checkpoint
  // 4 = lap

  unsigned int collision_type;
  glm::vec3 scale;
  glm::vec3 position; // center of the cube
  // no orientation for now
};
