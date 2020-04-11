#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec3 pos;
varying vec3 nor;
varying vec2 uv;

void main()
{
    pos = aPos;
	nor = aNor;
	uv = aUV;
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	
}
