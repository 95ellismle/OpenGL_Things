#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec3 posIncr;
out vec3 vertexColor;
out vec3 vertexPos;

void main()
{
    vertexPos = vec3(aPos - posIncr);
    gl_Position = vec4(vertexPos, 1.0);
    vertexColor = vec3(aColor - vertexPos);
}
