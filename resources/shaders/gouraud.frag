#version 450 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoords;
in float visibility;
in vec4 color;

out vec4 fragColor;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D textureDiffuse;
    sampler2D textureSpecular;
} material;

uniform vec4 skyColor;

void main()
{
    vec4 resultColor = color * texture(material.textureDiffuse, texCoords) + 0.0001 * texture(material.textureSpecular, texCoords);

    fragColor = mix(skyColor, resultColor, visibility);
}
