#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;

// Effects
uniform float bloomIntensity;
uniform float vignetteIntensity;
uniform float saturation;
uniform float contrast;
uniform float brightness;
uniform bool enableVignette;
uniform bool enableFilmGrain;
uniform float time;

// Random function for film grain
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
    // Get screen color
    vec3 color = texture(screenTexture, TexCoords).rgb;
    
    // Saturation adjustment
    if (saturation != 1.0) {
        float gray = dot(color, vec3(0.299, 0.587, 0.114));
        color = mix(vec3(gray), color, saturation);
    }
    
    // Contrast adjustment
    if (contrast != 1.0) {
        color = (color - 0.5) * contrast + 0.5;
    }
    
    // Brightness adjustment
    if (brightness != 0.0) {
        color += brightness;
    }
    
    // Vignette effect
    if (enableVignette) {
        vec2 center = TexCoords - 0.5;
        float dist = length(center);
        float vignette = 1.0 - smoothstep(0.3, 0.9, dist) * vignetteIntensity;
        color *= vignette;
    }
    
    // Film grain effect
    if (enableFilmGrain) {
        float grain = random(TexCoords + time) * 0.05;
        color += grain - 0.025;
    }
    
    // Clamp color
    color = clamp(color, 0.0, 1.0);
    
    FragColor = vec4(color, 1.0);
}
