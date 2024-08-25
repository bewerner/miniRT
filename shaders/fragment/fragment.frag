#version 330 core

in vec4	coord;
out vec4 FragColor;

void main()
{
    vec4 uv = coord;

    FragColor = vec4(uv.xyz, 1);
}
