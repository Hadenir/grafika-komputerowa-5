#version 450 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoords;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 texCoords;
out float visibility;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float fogDensity;
uniform float fogGradient;

void main()
{
    vec4 worldPos = modelMat * vec4(vertPos, 1.0);

    gl_Position = projMat * viewMat * worldPos;
    fragNormal = mat3(transpose(inverse(modelMat))) * vertNormal;
    fragPos = vec3(modelMat * vec4(vertPos, 1.0));
    texCoords = vertTexCoords;

    vec4 posRelativeToCamera = viewMat * worldPos;
    float distToCamera = length(posRelativeToCamera.xyz);
    visibility = clamp(exp(-pow(distToCamera * fogDensity, fogGradient)), 0.0, 1.0);
}