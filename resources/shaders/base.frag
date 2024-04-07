#version 330 core

in vec2 invertedTexCoord;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    color = mix(
        texture(texture1, invertedTexCoord), 
        texture(texture2, invertedTexCoord), 
        0.2);
}
