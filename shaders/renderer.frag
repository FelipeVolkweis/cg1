#version 460 core
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

    float farPlane;
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

    mat4 lightSpaceMatrix;
};

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 fragPos);  
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int index);  
vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir, int index);
float shadowCalculation(vec3 fragPosWorldSpace, vec3 normal, vec3 lightDir);
float sampleShadowCascade(vec3 offsetPosWorld, int layer, vec3 normal, vec3 lightDir);
float spotlightShadowCalculation(vec4 fragPosLightSpace, int index, vec3 normal, vec3 lightDir);
float pointLightShadowCalculation(vec3 fragPos, vec3 lightPos, int index, float farPlane, float nearPlane);

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform Material material;
uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform bool hasDirectionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform Spotlight spotlights[MAX_SPOTLIGHTS];
uniform int numSpotlights;

uniform sampler2DArrayShadow spotlightShadowMap;
uniform samplerCubeArray pointLightShadowMap;

uniform sampler2DArrayShadow shadowMap;
layout (std140) uniform LightSpaceMatrices {
    mat4 lightSpaceMatrices[16];
};
uniform float cascadePlaneDistances[16];
uniform int cascadeCount; 
uniform mat4 view;
uniform float zFar;

void main() {
    vec3 result = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    if (hasDirectionalLight) {
        result += calculateDirectionalLight(directionalLight, norm, viewDir, FragPos);
    }

    for (int i = 0; i < numPointLights; i++) {
        result += calculatePointLight(pointLights[i], norm, FragPos, viewDir, i);
    }

    for (int i = 0; i < numSpotlights; i++) {
        result += calculateSpotlight(spotlights[i], norm, FragPos, viewDir, i);
    }

    FragColor = vec4(result, material.dissolve);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(-light.direction);
    float diffuseAmount = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float incomingLight = max(dot(normal, halfwayDir), 0.0);
    float specularAmount = pow(incomingLight, material.shininess);

    vec3 specular = light.specular * specularAmount * vec3(texture(material.specular, TexCoord));
    vec3 diffuse = light.diffuse * diffuseAmount * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    
    float shadow = shadowCalculation(fragPos, normal, lightDir);

    return ((specular + diffuse) * (1.0 - shadow) + ambient);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, int index) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diffuseAmount = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float incomingLight = max(dot(normal, halfwayDir), 0.0);
    float specularAmount = pow(incomingLight, material.shininess);

    float d = length(light.position - fragPos);
    float den = light.constant + light.linear * d + light.quadratic * d * d;
    float attenuation = 1.0 / den;

    float shadow = pointLightShadowCalculation(fragPos, light.position, index, light.farPlane, 0.5);

    vec3 specular = light.specular * specularAmount * vec3(texture(material.specular, TexCoord));
    vec3 diffuse = light.diffuse * diffuseAmount * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    return ((specular + diffuse) * (1.0 - shadow) + ambient) * attenuation;
} 

vec3 calculateSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir, int index) {
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

    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(fragPos, 1.0);
    float shadow = spotlightShadowCalculation(fragPosLightSpace, index, normal, lightDir);

    vec3 specular = light.specular * specularAmount * vec3(texture(material.specular, TexCoord));
    vec3 diffuse = light.diffuse * diffuseAmount * vec3(texture(material.diffuse, TexCoord));
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    return ((specular + diffuse) * (1.0 - shadow) + ambient) * attenuation * intensity;
}

float sampleShadowCascade(vec3 offsetPosWorld, int layer, vec3 normal, vec3 lightDir) {
    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(offsetPosWorld, 1.0);
    
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0) {
        return 0.0;
    }

    float bias = 0.0005;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    
    float litFactor = 0.0;
    int samples = 0;
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            
            litFactor += texture(shadowMap, vec4(projCoords.xy + offset, layer, projCoords.z - bias));
            samples++;
        }    
    }
    
    litFactor /= float(samples);
        
    return 1.0 - litFactor; 
}

float shadowCalculation(vec3 fragPosWorldSpace, vec3 normal, vec3 lightDir) {
    vec4 fragPosViewSpace = view * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i) {
        if (depthValue < cascadePlaneDistances[i]) {
            layer = i;
            break;
        }
    }
    if (layer == -1) {
        layer = cascadeCount;
    }

    float cascadeDistance = (layer == cascadeCount) ? zFar : cascadePlaneDistances[layer];
    float normalOffsetScale = (1.0 - max(dot(normal, lightDir), 0.0)) * (0.001 * cascadeDistance);
    vec3 offsetPosWorld = fragPosWorldSpace + normal * normalOffsetScale;
    float shadow = sampleShadowCascade(offsetPosWorld, layer, normal, lightDir);

    if (layer < cascadeCount) {
        float cascadeFar = cascadePlaneDistances[layer];
        
        float blendBand = cascadeFar * 0.1; 
        float distanceToCascadeEdge = cascadeFar - depthValue;

        if (distanceToCascadeEdge < blendBand) {
            float nextShadow = sampleShadowCascade(offsetPosWorld, layer + 1, normal, lightDir);
            
            float blendFactor = smoothstep(blendBand, 0.0, distanceToCascadeEdge);
            shadow = mix(shadow, nextShadow, blendFactor);
        }
    }

    return shadow;
}

float spotlightShadowCalculation(vec4 fragPosLightSpace, int index, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0) {
        return 0.0;
    }

    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    vec2 texelSize = 1.0 / vec2(textureSize(spotlightShadowMap, 0));
    
    float litFactor = 0.0;
    int samples = 0;
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            litFactor += texture(spotlightShadowMap, vec4(projCoords.xy + offset, index, projCoords.z - bias));
            samples++;
        }    
    }
    
    litFactor /= float(samples);

    return 1.0 - litFactor; 
}

float pointLightShadowCalculation(vec3 fragPos, vec3 lightPos, int index, float farPlane, float nearPlane) {
    vec3 fragToLight = fragPos - lightPos;
    float dist = length(fragToLight);

    if (dist > farPlane)
        return 0.0;

    float fadeStart = farPlane * 0.9;
    float fadeFactor = 1.0 - smoothstep(fadeStart, farPlane, dist);

    vec3 sampleOffsetDirections[20] = vec3[](
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    float diskRadius = 0.02;
    float shadow = 0.0;

    for (int i = 0; i < 20; ++i) {
        vec3 sampleDir = fragToLight + sampleOffsetDirections[i] * diskRadius;

        float storedDepth = texture(pointLightShadowMap, vec4(sampleDir, index)).r;

        float storedZEye = nearPlane * farPlane / (farPlane - storedDepth * (farPlane - nearPlane));

        float dominantAxis = max(abs(sampleDir.x), max(abs(sampleDir.y), abs(sampleDir.z)));
        float storedDist = storedZEye * length(sampleDir) / dominantAxis;

        float bias = 0.15;
        if (dist > storedDist + bias)
            shadow += 1.0;
    }

    shadow /= 20.0;
    return shadow * fadeFactor;
}
