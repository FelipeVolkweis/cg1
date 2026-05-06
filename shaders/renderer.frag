#version 330 core
#define MAX_POINT_LIGHTS 32
#define MAX_SPOTLIGHTS 16

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

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform Material material;
uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform Spotlight spotlights[MAX_SPOTLIGHTS];
uniform int numSpotlights;

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

    return (specular + diffuse + ambient);
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
