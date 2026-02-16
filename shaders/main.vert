#version 450 core

// Vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

// Outputs to fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out mat3 TBN;
out float FogFactor;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

// Fog
uniform bool fogEnabled;
uniform float fogNear;
uniform float fogFar;

void main() {
    // Calculate world position
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // Calculate normal matrix
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    
    // Transform vectors to world space
    Normal = normalize(normalMatrix * aNormal);
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 B = normalize(normalMatrix * aBitangent);
    TBN = mat3(T, B, Normal);
    
    // Pass texture coordinates
    TexCoords = aTexCoords;
    
    // Calculate fog factor
    if (fogEnabled) {
        vec4 viewPos4 = view * worldPos;
        float dist = -viewPos4.z;
        FogFactor = clamp((fogFar - dist) / (fogFar - fogNear), 0.0, 1.0);
    } else {
        FogFactor = 1.0;
    }
    
    // Calculate final position
    gl_Position = projection * view * worldPos;
}
