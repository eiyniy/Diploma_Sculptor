#version 330 core

in vec3 positionModel;

out vec4 color;

void main()
{
    vec3 xTangent = dFdx(positionModel);
    vec3 yTangent = dFdy(positionModel);
    vec3 faceNormal = normalize(cross(xTangent, yTangent));

    vec3 faceNormalColor = vec3((faceNormal.x + 1) / 2, (faceNormal.y + 1) / 2, (faceNormal.z + 1) / 2);
    color = vec4(faceNormalColor, 1.0F);
}
