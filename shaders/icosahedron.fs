#version 330 core

out vec4 FragColor;

in vec3 pos;

void main()
{
    //float dzdy = dFdy(pos.z);
    //float dzdx = dFdx(pos.z);
    //FragColor = vec4(0, 200 * dzdx, 200 * dzdy, 1.0f);
    FragColor = vec4(0.0f);
}
