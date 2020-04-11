#version 330 core

out vec4 FragColor;

varying vec3 pos;
varying vec3 nor;
varying vec2 uv;

void main()
{
    FragColor = vec4(1);
    FragColor = vec4(uv, 1.0f, 1.0f);
}
