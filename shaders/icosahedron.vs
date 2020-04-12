#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aBary;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 pos;
out vec3 pos_screen;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	pos = (model * vec4(aPos, 1)).xyz;
	pos_screen = gl_Position.xyz;
}
