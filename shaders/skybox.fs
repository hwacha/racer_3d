#version 330 core

varying vec3 pos;
out vec4 FragColor;

void main()
{
    FragColor = vec4(pos.y, 0.0f, 2 * pos.y, 1.0f);
}
