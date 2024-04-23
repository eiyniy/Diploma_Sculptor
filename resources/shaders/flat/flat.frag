#version 330 core

in vec3 positionModel;

out vec4 color;

uniform vec3 cameraPos;

uniform vec3 lightColor;

uniform float dimmingFactor;
uniform bool isDistanceDimming;

void main()
{
    vec3 xTangent = dFdx(positionModel);
    vec3 yTangent = dFdy(positionModel);
    vec3 faceNormal = normalize(cross(xTangent, yTangent));
    
    vec3 lightDir = normalize(cameraPos - positionModel);
    float lightValue = max(dot(lightDir, faceNormal), 0.F);

    if (isDistanceDimming) {
        float dimmingValue = max((distance(cameraPos, positionModel) * dimmingFactor), 1.F);
        lightValue /= dimmingValue;
    }

    color = vec4(lightValue * lightColor, 1.0F);
}
