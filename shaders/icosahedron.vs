#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aBary;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 pos;
out vec3 pos_screen;
out vec3 bary;
out vec3 normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    pos = (model * vec4(aPos, 1.0)).xyz;
    pos_screen = gl_Position.xyz;
    bary = aBary;
    normal = (model * vec4(aNormal, 0.0)).xyz;
}
