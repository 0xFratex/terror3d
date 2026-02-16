#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 particlePos;
uniform float scale;

void main() {
    TexCoords = aTexCoords;
    
    // Billboard: position in view space, then offset
    vec3 viewPos = (view * vec4(particlePos, 1.0)).xyz;
    vec3 offset = aPos * scale;
    
    gl_Position = projection * vec4(viewPos + offset, 1.0);
}
