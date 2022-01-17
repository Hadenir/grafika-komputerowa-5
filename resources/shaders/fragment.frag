#version 450 core

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = 0.5 * pow(viewAngle, 32);
    vec3 specular = specularStrength * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = vec4(result, 1.0);
}