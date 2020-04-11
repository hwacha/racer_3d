#ifndef PLAYER_H
#define PLAYER_H
#include "player.h"
#endif

Player initial_player() {
    struct Player player;
    player.position = glm::vec3(0.0f);
    player.velocity = glm::vec3(0.0f);
    player.heading = glm::vec3(1.0f, 0.0f, 0.0f);
    return player;
}

void step_player(PlayerInputs input, Player *player) {
    // make frame
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // assuming purely planar movement
    glm::vec3 forward = glm::normalize(player->heading);
    glm::vec3 right = glm::cross(forward, up);

    // apply forward acceleration
    float accel = 0.0;
    if (input.key_w_pressed) {
        accel += 0.001f;
    }
    if (input.key_s_pressed) {
        accel -= 0.001f;
    }
    player->velocity += accel * forward;

    // apply turning
    float ang_accel = 0.0f;
    if (input.key_a_pressed) {
        ang_accel += 0.05f;
    }
    if (input.key_d_pressed) {
        ang_accel -= 0.05f;
    }

    glm::mat4 rotation;
    rotation = glm::rotate(glm::mat4(1.0f), ang_accel, up);
    glm::vec4 v = rotation * glm::vec4(player->velocity, 1.0f); // don't know how to get a 3x3 rotation
    player->velocity = glm::vec3(v.x, v.y, v.z);

    // step position
    player->velocity *= 1.0f - 0.02f; // drag
    player->position += player->velocity;

    // update heading
    if (player->velocity != glm::vec3(0.0f)) {
        player->heading = player->velocity;
    }

    return;
}
