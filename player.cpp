#ifndef PLAYER_H
#define PLAYER_H
#include "player.h"
#endif

#define _USE_MATH_DEFINES
#include <math.h>

Player initial_player(unsigned int id, glm::vec2 initial_position) {
    struct Player player;
    player.id = id;
    player.position = glm::vec3(initial_position.x, 0.5f, initial_position.y);
    player.speed = 0.0f;
    player.orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    player.pitch_rads = 0.0f; // Ack! Euler angles!
    player.yaw_rads = -M_PI * 0.5f;
    return player;
}

void step_player(PlayerInputs input, Player *player) {
    // apply forward acceleration
    float accel = 0.0;
    if (input.key_w_pressed && player->id == 1) {
        accel += 0.025f;
    }
    if (input.key_s_pressed && player->id == 1) {
        accel -= 0.025f;
    }
    if (input.key_up_pressed && player->id == 2) {
        accel += 0.025f;
    }
    if (input.key_down_pressed && player->id == 2) {
        accel -= 0.025f;
    }
    player->speed += accel;

    // apply turning
    float ang_velocity = 0.0f;
    if (input.key_a_pressed && player->id == 1) {
        ang_velocity += 0.05f;
    }
    if (input.key_d_pressed && player->id == 1) {
        ang_velocity -= 0.05f;
    }
    if (input.key_right_pressed && player->id == 2) {
        ang_velocity -= 0.05f;
    }
    if (input.key_left_pressed && player->id == 2) {
        ang_velocity += 0.05f;
    }

    // step position
    player->speed *= 1.0f - 0.125f; // drag
    player->position += player->forward() * player->speed;

    // pitch and yaw and orientation
    float radius = 0.5f; // needs to match the rendering radius
    float rotation_angle = player->speed / radius;
    float tau = 2.0f*M_PI;

    // update orientation
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 globalRotationAxis = glm::cross(player->forward(), up);
    float rotation_factor = 0.33f; // artificially slow the rotation for looks
    glm::quat globalRotation = glm::rotate(
        glm::mat4(1.0f),
        -rotation_angle * rotation_factor,
        globalRotationAxis);
    player->orientation = globalRotation * player->orientation;

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
