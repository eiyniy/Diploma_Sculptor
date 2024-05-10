#version 330 core

#define SMOOTH(r) (mix(1.F, 0.F, smoothstep(0.9F, 1.F, r)))
#define M_PI 3.1415926535897932384626433832795F

out vec4 color;

uniform vec2 uResolution;   // viewport resolution (in pixels)
uniform float uTime;        // shader playback time (in seconds)

float movingRing(vec2 uv, vec2 center, float r1, float r2)
{
    vec2 d = uv - center;
    float r = sqrt(dot(d, d));
    d = normalize(d);
    float theta = -atan(d.y, d.x);
    theta = mod(-uTime + 0.5F * (1.F + theta / M_PI), 1.F);
    //anti aliasing for the ring's head (thanks to TDM !)
    theta -= max(theta - 1.F + 1e-2, 0.F) * 1e2;
    return theta * (SMOOTH(r / r2) - SMOOTH(r / r1));
}

void main()
{
	vec2 uv = gl_FragCoord.xy;
    float ring = movingRing(
        uv, 
        vec2(uResolution.x / 2.F, uResolution.y / 2.F), 
        20.F, 
        30.F);
    // color = vec4(0.F, 1.F, 0.F, 1.F);
    color = vec4(0.1F + 0.9F * ring);
}