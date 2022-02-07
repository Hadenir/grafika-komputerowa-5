#version 450 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoords;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 texCoords;
out float visibility;
out vec4 color;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float fogDensity;
uniform float fogGradient;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D textureDiffuse;
    sampler2D textureSpecular;
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
} spotLights[80];

uniform int numSpotLights;

uniform vec3 viewPos;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

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

    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 resultColor = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < numDirLights; i++)
    {
        resultColor += calculateDirectionalLight(dirLights[i], normal, viewDir);
    }
    for (int i = 0; i < numPointLights; i++)
    {
        resultColor += calculatePointLight(pointLights[i], normal, fragPos, viewDir);
    }
    for (int i = 0; i < numSpotLights; i++)
    {
        resultColor += calculateSpotLight(spotLights[i], normal, fragPos, viewDir);
    }
    color = vec4(resultColor, 1.0);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 lightDir = normalize(-light.direction);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * light.diffuse * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = specularStrength * light.specular * material.specular;

    return max(ambient + diffuse + specular, vec3(0.0));
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * light.diffuse * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = specularStrength * light.specular * material.specular;

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.kConstant + light.kLinear * dist + light.kQuadratic * dist * dist);

    return max((ambient + diffuse + specular) * attenuation, vec3(0.0));
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * material.ambient;

    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * light.diffuse * material.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float viewAngle = max(dot(viewDir, reflectDir), 0.0);
    float specularStrength = pow(viewAngle, material.shininess);
    vec3 specular = specularStrength * light.specular * material.specular;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.kConstant + light.kLinear * dist + light.kQuadratic * dist * dist);

    return max((ambient + (diffuse + specular) * intensity) * attenuation, vec3(0.0));
}