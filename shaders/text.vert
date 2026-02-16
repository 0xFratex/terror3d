#version 450 core

layout (location = 0) in vec4 aVertex; // <vec2 pos, vec2 tex>

out vec2 TexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aVertex.xy, 0.0, 1.0);
    TexCoords = aVertex.zw;
}
