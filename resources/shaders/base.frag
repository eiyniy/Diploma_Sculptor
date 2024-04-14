#version 330 core

in vec2 invertedTexCoord;
in vec3 _color;

out vec4 color;

uniform sampler2D containerTexture;
uniform sampler2D faceTexture;

void main()
{
    color = mix(
        texture(containerTexture, invertedTexCoord), 
        texture(faceTexture, invertedTexCoord), 
        0.2) * vec4(_color, 1.0);
}
