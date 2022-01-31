#version 450 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

uniform struct Material {
    sampler2D textureDiffuse;
    sampler2D textureSpecular;
    float shininess;
} material;

uniform struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
} dirLights[10];

uniform int numDirLights;

uniform struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float kConstant;
    float kLinear;
    float kQuadratic;
} pointLights[10];

uniform int numPointLights;

uniform struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float kConstant;
    float kLinear;
    float kQuadratic;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;
} spotLights[10];

uniform int numSpotLights;

uniform vec3 viewPos;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 resultColor = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < numDirLights; i++)
        resultColor += calculateDirectionalLight(dirLights[i], normal, viewDir);
    for(int i = 0; i < numPointLights; i++)
        resultColor += calculatePointLight(pointLights[i], normal, fragPos, viewDir);
    for(int i = 0; i < numSpotLights; i++)
        resultColor += calculateSpotLight(spotLights[i], normal, fragPos, viewDir);

    fragColor = vec4(resultColor, 1.0);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * texture(material.textureDiffuse, texCoords).rgb;

    vec3 lightDir = normalize(-light.direction);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * texture(material.textureDiffuse, texCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = light.specular * specularStrength * texture(material.textureSpecular, texCoords).rgb;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * texture(material.textureDiffuse, texCoords).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * texture(material.textureDiffuse, texCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = light.specular * specularStrength * texture(material.textureSpecular, texCoords).rgb;

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.kConstant + light.kLinear * dist + light.kQuadratic * dist * dist);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * texture(material.textureDiffuse, texCoords).rgb;

    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * texture(material.textureDiffuse, texCoords).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = light.specular * specularStrength * texture(material.textureSpecular, texCoords).rgb;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.kConstant + light.kLinear * dist + light.kQuadratic * dist * dist);

    return (ambient + (diffuse + specular) * intensity) * attenuation;
}