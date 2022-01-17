#version 450 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

uniform struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
} material;

uniform struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

uniform vec3 viewPos;

void main()
{
    vec3 ambient = texture(material.diffuse, texCoords).xyz * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = texture(material.diffuse, texCoords).xyz * diffuseStrength * light.diffuse;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = texture(material.specular, texCoords).xyz * specularStrength * light.specular;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}