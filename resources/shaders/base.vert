#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec3 positionModel;
out vec3 normalColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * position;

    positionModel = (model * position).xyz;
    normalColor = vec3((normal.x + 1) / 2, (normal.y + 1) / 2, (normal.z + 1) / 2);
}
