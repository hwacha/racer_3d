#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int effect_type;
uniform float effect_time;

out vec3 nor;
out vec2 TexCoords;

void main()
{
	vec3 aapos = aPos;
	aapos.z = 0;
    nor = aNor;
    float time_slot = effect_time / 60 ;

    if (effect_type == 1) { 
        vec2 texSector = vec2(
            mod(floor(time_slot / 0.1), 4.0f),
            floor(floor(time_slot / 0.1) / 4.0f));
        TexCoords = (aTexCoords + texSector) / 4.0f;
    } else if (effect_type == 2) {
        TexCoords = vec2(-1, -1) * aTexCoords;
    } else {
        TexCoords = aTexCoords;
    }
    gl_Position = projection * view * model * vec4(aapos, 1.0);
}
