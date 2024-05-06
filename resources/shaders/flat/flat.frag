#version 330 core

in vec3 positionModel;

out vec4 color;

uniform vec3 uCameraPos;

uniform vec3 uLightColor;

uniform float uDimmingFactor;
uniform bool uIsDistanceDimming;

void main()
{
    vec3 xTangent = dFdx(positionModel);
    vec3 yTangent = dFdy(positionModel);
    vec3 faceNormal = normalize(cross(xTangent, yTangent));
    
    vec3 lightDir = normalize(uCameraPos - positionModel);
    float lightValue = max(dot(lightDir, faceNormal), 0.F);

    if (uIsDistanceDimming) {
        float dimmingValue = max((distance(uCameraPos, positionModel) * uDimmingFactor), 1.F);
        lightValue /= dimmingValue;
    }

    color = vec4(lightValue * uLightColor, 1.0F);
}
