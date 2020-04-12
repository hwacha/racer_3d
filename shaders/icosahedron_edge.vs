#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 scaled_pos = 1.01 * aPos;
    gl_Position = projection * view * model * vec4(scaled_pos, 1.0);
    pos = (model * vec4(aPos, 1)).xyz;
}
