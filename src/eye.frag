#version 330 core
out vec4 FragColor;
in vec2 texCoord;

;
// TODO use squared distance instead of real one to avoid sqrt

const vec3 white = vec3(1.0f, 1.0f, 1.0f);
const vec3 black = vec3(0.0f, 0.0f, 0.0f);
const vec3 eyeColor = vec3(0.2f, 0.5f, 1.f);

float random(vec3 vec)
{
    float random = dot(vec, vec3(12.9898, 78.233, 37.719));
    random = fract(random);
    return random;
}

vec2 noiseVector()
{
    const float noiseScaling = 0.001f;
    vec3 seed1 = vec3(texCoord.x, texCoord.y, texCoord.x);
    vec3 seed2 = vec3(texCoord.y, texCoord.x, texCoord.y);
    return vec2(random(seed1), random(seed2)) * noiseScaling;
}

vec3 computeIris(vec2 eyeCenter)
{
    const vec2 eyeBaseAngle = vec2(1.0f, 0.0f);
    const float angleScaling = 150.0f;

    vec2 noisedPosition = texCoord + noiseVector();

    float dotAngle = dot(normalize(noisedPosition - eyeCenter), eyeBaseAngle);
    float angle = acos(dotAngle);
    return eyeColor * cos(angle * angleScaling);
}

vec3 computeProceduralEye(vec2 eyeCenter)
{
    const float pupilRadius = 0.07f;
    const float irisRadius = 0.17f;

    float currentRadius = length(texCoord - eyeCenter);
    if (currentRadius > irisRadius)
        return white;
    else if (currentRadius > pupilRadius)
        return computeIris(eyeCenter);
    return black;
}

void main()
{
    const vec2 eyeCenter = vec2(0.5f, 0.5f);
    FragColor = vec4(computeProceduralEye(eyeCenter), 1.0f);
}
