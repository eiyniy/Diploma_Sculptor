#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel* position;
}
