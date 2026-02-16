#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D particleTexture;
uniform vec4 particleColor;
uniform float alpha;

void main() {
    vec4 texColor = texture(particleTexture, TexCoords);
    FragColor = texColor * particleColor * alpha;
    
    if (FragColor.a < 0.01) discard;
}
