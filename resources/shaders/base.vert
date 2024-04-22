#version 330 core

layout (location = 0) in vec4 position;
// layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec2 invertedTexCoord;
out vec3 _color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * position;
    
    // _color = color;
    // _color = vec3(0.8f, 0.1f, 0.1f);
    _color = vec3(1.f, 1.f, 1.f);
    invertedTexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}
