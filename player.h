// Todo: Assuming that the player is the only entity
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef INPUT
#define INPUT
#include "input.h"
#endif

struct Player {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 heading; // in case velocity is zero, should always be zero
    float pitch_rads;
    float yaw_rads;
};

Player initial_player();
void step_player(PlayerInputs input, Player *player);
