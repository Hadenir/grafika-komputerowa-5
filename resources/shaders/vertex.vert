#version 450 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoords;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(vertPos, 1.0);
    fragNormal = mat3(transpose(inverse(modelMat))) * vertNormal;
    fragPos = vec3(modelMat * vec4(vertPos, 1.0));
    texCoords = vertTexCoords;
}