#version 330 core
#define MAX_POINT_LIGHTS 32
#define MAX_SPOTLIGHTS 16
#define MIN_SHADOW_BIAS 0.005f

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float dissolve;
};

struct DirectionalLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Spotlight {    
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);  
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float shadowCalculation(vec3 normal, vec3 lightDir);

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform Material material;
uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform Spotlight spotlights[MAX_SPOTLIGHTS];
uniform int numSpotlights;
uniform sampler2D shadowMap;

void main() {
    vec3 output = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    output += calculateDirectionalLight(directionalLight, norm, viewDir);

    for (int i = 0; i < numPointLights; i++) {
        output += calculatePointLight(pointLights[i], norm, FragPos, viewDir);
    }

    for (int i = 0; i < numSpotlights; i++) {
        output += calculateSpotlight(spotlights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(output, material.dissolve);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diffuseAmount = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float incomingLight = max(dot(normal, halfwayDir), 0.0);
    float specularAmount = pow(incomingLight, material.shininess);

    vec3 specular = light.specular * specularAmount * vec3(texture(material.specular, TexCoord));
    vec3 diffuse = light.diffuse * diffuseAmount * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    float shadow = shadowCalculation(normal, lightDir);

    return ((specular + diffuse) * (1.0 - shadow) + ambient);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseAmount = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float incomingLight = max(dot(normal, halfwayDir), 0.0);
    float specularAmount = pow(incomingLight, material.shininess);

    float d = length(light.position - fragPos);
    float den = light.constant + light.linear * d + light.quadratic * d * d;
    float attenuation = 1.0 / den;

    vec3 specular = light.specular * specularAmount * vec3(texture(material.specular, TexCoord));
    vec3 diffuse = light.diffuse * diffuseAmount * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    return (specular + diffuse + ambient) * attenuation;
} 

vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseAmount = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float incomingLight = max(dot(normal, halfwayDir), 0.0);
    float specularAmount = pow(incomingLight, material.shininess);

    float d = length(light.position - fragPos);
    float den = light.constant + light.linear * d + light.quadratic * d * d;
    float attenuation = 1.0 / den;

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 specular = light.specular * specularAmount * vec3(texture(material.specular, TexCoord));
    vec3 diffuse = light.diffuse * diffuseAmount * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    return (specular + diffuse + ambient) * attenuation * intensity;
}

float shadowCalculation(vec3 normal, vec3 lightDir) {
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), MIN_SHADOW_BIAS);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}