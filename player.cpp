#ifndef PLAYER_H
#define PLAYER_H
#include "player.h"
#endif

#define _USE_MATH_DEFINES
#include <math.h>

Player initial_player() {
    struct Player player;
    player.position = glm::vec3(0.0f, 0.5f, 0.0f);
    player.speed = 0.0f;
    player.pitch_rads = 0.0; // Ack! Euler angles!
    player.yaw_rads = 0.0;
    return player;
}

void step_player(PlayerInputs input, Player *player) {
    // apply forward acceleration
    float accel = 0.0;
    if (input.key_w_pressed) {
        accel += 0.025f;
    }
    if (input.key_s_pressed) {
        accel -= 0.025f;
    }
    player->speed += accel;

    // apply turning
    float ang_velocity = 0.0f;
    if (input.key_a_pressed) {
        ang_velocity += 0.05f;
    }
    if (input.key_d_pressed) {
        ang_velocity -= 0.05f;
    }

    // step position
    player->speed *= 1.0f - 0.125f; // drag
    player->position += player->forward() * player->speed;

    // pitch and yaw
    float radius = 0.5f; // needs to match the rendering radius
    float tau = 2.0f*M_PI;

    // update pitch
    float pitch_rads_new = fmod(
        player->pitch_rads + player->speed / radius, // can only roll forward
        tau);
    player->pitch_rads = pitch_rads_new;

    // update yaw
    player->yaw_rads = fmod(player->yaw_rads + ang_velocity, tau);
    // TODO: update velocity computations to use yaw, remove heading

    return;
}
