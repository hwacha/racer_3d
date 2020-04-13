#version 330 core

out vec4 FragColor;

in vec3 nor;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float effect_time;

void main()
{
    float r = 0.03;
    FragColor = vec4(
        1.3 - r * effect_time,
        1 - r * effect_time,
        1 - r * effect_time,
        1 - r * effect_time);
    FragColor = texture2D(texture_diffuse1, TexCoords);
}
