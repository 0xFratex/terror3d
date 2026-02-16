#version 450 core

// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;
in float FogFactor;

// Output
out vec4 FragColor;

// Material
struct Material {
    sampler2D diffuseMap;
    sampler2D normalMap;
    sampler2D specularMap;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

uniform Material material;

// Light structure
struct Light {
    int type;           // 0=directional, 1=point, 2=spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float range;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
    bool castShadows;
};

// Uniforms
uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform int numLights;
uniform Light lights[16];
uniform vec3 fogColor;
uniform bool fogEnabled;
uniform bool useNormalMap;
uniform bool useSpecularMap;

// Calculate lighting contribution
vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 result = vec3(0.0);
    
    // Directional light
    if (light.type == 0) {
        vec3 lightDir = normalize(-light.direction);
        
        // Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        
        // Specular (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
        
        result = light.color * (diff * material.diffuseColor + spec * material.specularColor);
    }
    // Point light
    else if (light.type == 1) {
        vec3 lightDir = normalize(light.position - fragPos);
        
        // Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        
        // Specular (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
        
        // Attenuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
                                    light.quadratic * distance * distance);
        
        // Range check
        if (light.range > 0.0 && distance > light.range) {
            attenuation = 0.0;
        }
        
        result = light.color * (diff * material.diffuseColor + spec * material.specularColor) * attenuation;
    }
    // Spot light
    else if (light.type == 2) {
        vec3 lightDir = normalize(light.position - fragPos);
        
        // Check if within cone
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.innerCutoff - light.outerCutoff;
        float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
        
        if (intensity > 0.0) {
            // Diffuse
            float diff = max(dot(normal, lightDir), 0.0);
            
            // Specular (Blinn-Phong)
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
            
            // Attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * distance + 
                                        light.quadratic * distance * distance);
            
            // Range check
            if (light.range > 0.0 && distance > light.range) {
                attenuation = 0.0;
            }
            
            result = light.color * (diff * material.diffuseColor + spec * material.specularColor) 
                     * intensity * attenuation;
        }
    }
    
    return result;
}

void main() {
    // Get base color from texture or material
    vec4 texColor = texture(material.diffuseMap, TexCoords);
    vec3 baseColor = texColor.rgb * material.diffuseColor;
    
    // Get normal (optionally from normal map)
    vec3 normal = normalize(Normal);
    if (useNormalMap) {
        normal = texture(material.normalMap, TexCoords).rgb;
        normal = normal * 2.0 - 1.0;
        normal = normalize(TBN * normal);
    }
    
    // Get specular
    vec3 specColor = material.specularColor;
    if (useSpecularMap) {
        specColor = texture(material.specularMap, TexCoords).rgb;
    }
    
    // View direction
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Start with ambient
    vec3 lighting = ambientColor * baseColor;
    
    // Add light contributions
    for (int i = 0; i < numLights && i < 16; i++) {
        lighting += calculateLight(lights[i], normal, FragPos, viewDir);
    }
    
    // Apply fog
    vec3 finalColor = lighting;
    if (fogEnabled) {
        finalColor = mix(fogColor, lighting, FogFactor);
    }
    
    // Output
    FragColor = vec4(finalColor, texColor.a);
    
    // Reinhard tone mapping (optional)
    // finalColor = finalColor / (finalColor + vec3(1.0));
    // FragColor = vec4(finalColor, texColor.a);
}
