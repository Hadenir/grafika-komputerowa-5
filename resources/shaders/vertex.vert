#version 450 core
layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec2 vertTexCoords;

out vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
    gl_Position = projMat * viewMat * modelMat * vec4(vertPosition, 1.0);
    texCoords = vertTexCoords;
}