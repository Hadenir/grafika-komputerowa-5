#version 450 core
layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertColor;
layout (location = 2) in vec2 vertTexCoords;

out vec3 color;
out vec2 texCoords;

void main()
{
    gl_Position = vec4(vertPosition, 1.0);
    color = vertColor;
    texCoords = vertTexCoords;
}