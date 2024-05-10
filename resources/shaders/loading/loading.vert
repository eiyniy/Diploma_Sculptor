#version 330 core

layout (location = 0) in vec2 position;

// const vec2 quadVertices[4] = { vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) };

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}