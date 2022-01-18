#version 450 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

uniform struct Material {
    sampler2D texture0;
    sampler2D texture1;
    float shininess;
} material;

uniform struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} dirLight;

uniform struct PointLight {
    vec3 position;
    float kConstant;
    float kLinear;
    float kQuadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} pointLight;

uniform struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
    float kConstant;
    float kLinear;
    float kQuadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} spotLight;

uniform vec3 viewPos;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 resultColor = calculateDirectionalLight(dirLight, normal, viewDir);
    resultColor += calculatePointLight(pointLight, normal, fragPos, viewDir);
    resultColor += calculateSpotLight(spotLight, normal, fragPos, viewDir);

    fragColor = vec4(resultColor, 1.0);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * texture(material.texture0, texCoords).rgb;

    vec3 lightDir = normalize(-light.direction);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * texture(material.texture0, texCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = light.specular * specularStrength * texture(material.texture1, texCoords).rgb;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * texture(material.texture0, texCoords).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * texture(material.texture0, texCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = light.specular * specularStrength * texture(material.texture1, texCoords).rgb;

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.kConstant + light.kLinear * dist + light.kQuadratic * dist * dist);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * texture(material.texture0, texCoords).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * texture(material.texture0, texCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = light.specular * specularStrength * texture(material.texture1, texCoords).rgb;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.kConstant + light.kLinear * dist + light.kQuadratic * dist * dist);

    return (ambient + (diffuse + specular) * intensity) * attenuation;
}