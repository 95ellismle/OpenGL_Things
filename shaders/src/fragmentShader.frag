#version 330 core
out vec4 FragColor;

uniform vec4 outColor; // This is set via OpenGL code

void main()
{
    FragColor = outColor;
}
