#version 330 core

uniform vec3 cameraPos;
uniform int player_id;

out vec4 FragColor;

in vec3 pos;
in vec3 pos_screen;
in vec3 bary;
in vec3 normal;

void main()
{
    float ratio = 1.00 / 1.52; // could be made a function of a wavelength
    vec3 I = normalize(pos - cameraPos);
    vec3 R = refract(I, normal, ratio);
    vec3 R_norm = (R + vec3(1.0f)) / 2.0f;
    vec3 face_color = vec3(1.0f) - 0.7 * R_norm*R_norm;

    if (player_id == 2) {
        face_color = vec3(1.0f) - face_color;
    }

    vec3 edge_color = vec3(1.0f);

    float dist = min(bary.x, min(bary.y, bary.z)); // distance from edge
    float factor = pow(1.0 - dist, 64);

    FragColor
        = factor         * vec4(edge_color, 1.0f)
        + (1.0 - factor) * vec4(face_color, 1.0f);
}
