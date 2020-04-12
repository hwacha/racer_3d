struct PlayerInputs {
    bool key_w_pressed;
    bool key_a_pressed;
    bool key_s_pressed;
    bool key_d_pressed;
    bool key_esc_pressed;
    bool key_up_pressed;
    bool key_down_pressed;
    bool key_left_pressed;
    bool key_right_pressed;
};

PlayerInputs poll_inputs(GLFWwindow *window);
