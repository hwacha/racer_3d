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
    unsigned int id;
    glm::vec3 position;
    float speed;
    float pitch_rads; // currently just for rolling animation
    float yaw_rads;
    glm::vec3 forward() {
	  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), yaw_rads, {0, 1, 0});
    // TODO pitch
    glm::vec4 res = rotation * glm::vec4{ 1, 0, 0, 1 };
    return { res.x, res.y, res.z };
	}
};

Player initial_player(unsigned int id);
void step_player(PlayerInputs input, Player *player);
