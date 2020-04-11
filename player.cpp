#ifndef PLAYER_H
#define PLAYER_H
#include "player.h"
#endif

#include <cmath>

Player initial_player() {
    struct Player player;
    player.position = glm::vec3(0.0f, 0.5f, 0.0f);
    player.velocity = glm::vec3(0.0f);
    player.heading = glm::vec3(1.0f, 0.0f, 0.0f);
    player.pitch_rads = 0.0; // Ack! Euler angles!
    player.yaw_rads = 0.0;
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
        accel += 0.025f;
    }
    if (input.key_s_pressed) {
        accel -= 0.025f;
    }
    player->velocity += accel * forward;

    // apply turning
    float ang_velocity = 0.0f;
    if (input.key_a_pressed) {
        ang_velocity += 0.05f;
    }
    if (input.key_d_pressed) {
        ang_velocity -= 0.05f;
    }

    glm::mat4 rotation;
    rotation = glm::rotate(glm::mat4(1.0f), ang_velocity, up);
    glm::vec4 v = rotation * glm::vec4(player->velocity, 1.0f); // don't know how to get a 3x3 rotation
    player->velocity = glm::vec3(v.x, v.y, v.z);

    // step position
    player->velocity *= 1.0f - 0.125f; // drag
    player->position += player->velocity;

    // update heading
    if (player->velocity != glm::vec3(0.0f)) {
        player->heading = glm::normalize(player->velocity);
    }

    // pitch and yaw
    float radius = 0.5f; // needs to match the rendering radius
    float tau = 2.0f*M_PI;

    // update pitch
    float pitch_rads_new = fmod(
        player->pitch_rads + glm::length(player->velocity) / radius, // can only roll forward
        tau);
    player->pitch_rads = pitch_rads_new;

    // update yaw
    player->yaw_rads = fmod(player->yaw_rads + ang_velocity, tau);
    // TODO: update velocity computations to use yaw, remove heading

    return;
}
