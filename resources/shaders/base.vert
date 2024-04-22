#version 330 core

layout (location = 0) in vec4 position;
layout (location = 2) in vec2 texCoord;

out vec2 invertedTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * position;

    //TODO: Do not invert in shader. Do it in ObjectBuilder::transform()
    invertedTexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}
