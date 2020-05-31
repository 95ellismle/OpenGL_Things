#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform vec3 posIncr;
uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 200*posIncr.x + 150*posIncr.y);
}
