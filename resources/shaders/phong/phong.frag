#version 330 core

in vec3 _normal;
in vec3 viewPosition;

out vec4 color;

uniform vec3 cameraPos;
uniform vec3 lightColor;

void main()
{
    vec3 xTangent = dFdx(viewPosition);
    vec3 yTangent = dFdy(viewPosition);
    vec3 faceNormal = normalize(cross(xTangent, yTangent));

    // color = vec4((faceNormal.x + 1) / 2, (faceNormal.y + 1) / 2, (faceNormal.z + 1) / 2, 1.0F);
    // color = vec4(_normal.xyz, 1.0F);

    //TODO: made uniformw
    vec3 lightColor = vec3(1, 1, 1);
    float fogFactor = 0.05F;
    
    vec3 lightDir = normalize(cameraPos - viewPosition);
    float lightValue = max(dot(lightDir, faceNormal), 0.F) / max((distance(cameraPos, viewPosition) * fogFactor), 1.F);

    color = vec4(lightValue * lightColor, 1.0F);
}
