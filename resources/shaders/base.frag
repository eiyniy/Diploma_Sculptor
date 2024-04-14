#version 330 core

in vec2 invertedTexCoord;

out vec4 color;

uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

void main()
{
    color = mix(
        texture(containerTexture, invertedTexCoord), 
        texture(faceTexture, invertedTexCoord), 
        0.2);
}
