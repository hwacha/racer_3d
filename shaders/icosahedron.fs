#version 330 core

out vec4 FragColor;

in vec3 pos;
in vec3 bary;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    //float dzdy = dFdy(pos.z);
    //float dzdx = dFdx(pos.z);
    //FragColor = vec4(0, 200 * dzdx, 200 * dzdy, 1.0f);
    float dist = min(bary.x, min(bary.y, bary.z)); // distance from edge
    if (dist < 0.01) {
        vec3 rgb = hsv2rgb(vec3(sin(pos.x), 1.0f, 1.0f));
        FragColor = vec4(rgb, 1.0f);
    } else {
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}
