struct PlayerInputs {
    bool key_w_pressed;
    bool key_a_pressed;
    bool key_s_pressed;
    bool key_d_pressed;
    bool key_esc_pressed;
};

PlayerInputs poll_inputs(GLFWwindow*);
