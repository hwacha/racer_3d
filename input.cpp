#include <GLFW/glfw3.h>

#include "input.h"

PlayerInputs poll_inputs(GLFWwindow *window) {

    struct PlayerInputs inputs;
    inputs.key_w_pressed = false;
    inputs.key_a_pressed = false;
    inputs.key_s_pressed = false;
    inputs.key_d_pressed = false;
    inputs.key_esc_pressed = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        inputs.key_w_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        inputs.key_a_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        inputs.key_s_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        inputs.key_d_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        inputs.key_esc_pressed = true;
    }

    return inputs;
}
