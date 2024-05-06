#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out vec3 positionModel;
out vec3 normalColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * position;

    positionModel = (uModel * position).xyz;
    normalColor = vec3((normal.x + 1.0F) / 2.0F, (normal.y + 1.0F) / 2.0F, (normal.z + 1.0F) / 2.0F);
}
