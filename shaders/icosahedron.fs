#version 330 core

out vec4 FragColor;

in vec3 pos;
in vec3 pos_screen;
in vec3 bary;
in vec3 normal;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    // WIP
    float dzdy = dFdy(pos.z);
    float dzdx = dFdx(pos.z);
    //vec4 normals = vec4(0.2f, 200 * dzdx, 200 * dzdy, 1.0);
    vec4 normals = vec4(normal, 1.0);

    //vec4 osc = vec4(hsv2rgb(vec3(hue, 1.0f, 1.0f)), 1.0f);
    vec4 osc = vec4(1.0f);

    float dist = min(bary.x, min(bary.y, bary.z)); // distance from edge
    float factor = pow(1.0 - dist, 32);

    FragColor = factor * osc + (1.0 - factor) * normals * 0.8;
}

//void main()
//{ 
//    float ratio = 1.00 / 1.52; // could be made a function of a wavelength
//    vec3 I = normalize(Position - cameraPos);
//    vec3 R = refract(I, normalize(Normal), ratio);
//    FragColor = vec4(texture(skybox, R).rgb, 1.0);
//} 

//https://en.wikipedia.org/wiki/Cauchy%27s_equation
