#include "graphics/Renderer.h"
#include "graphics/Shader.h"
#include "core/Logger.h"
#include "core/Config.h"
#include <GL/glew.h>
#include <sstream>

namespace ExperimentRedbear {

Renderer& Renderer::getInstance() {
    static Renderer instance;
    return instance;
}

bool Renderer::initialize(int width, int height) {
    m_width = width;
    m_height = height;

    // Enable features
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Set clear color (dark horror atmosphere)
    glClearColor(m_settings.clearColor.r, m_settings.clearColor.g, 
                 m_settings.clearColor.b, m_settings.clearColor.a);

    // Set up default shaders
    setupDefaultShaders();
    setupPostProcessing();

    m_initialized = true;
    LOG_INFO("Renderer initialized: " + std::to_string(width) + "x" + std::to_string(height));

    return true;
}

void Renderer::shutdown() {
    if (m_postFBO) {
        glDeleteFramebuffers(1, &m_postFBO);
        m_postFBO = 0;
    }
    if (m_postTexture) {
        glDeleteTextures(1, &m_postTexture);
        m_postTexture = 0;
    }
    if (m_postDepthBuffer) {
        glDeleteRenderbuffers(1, &m_postDepthBuffer);
        m_postDepthBuffer = 0;
    }
    if (m_quadVAO) {
        glDeleteVertexArrays(1, &m_quadVAO);
        m_quadVAO = 0;
    }
    if (m_quadVBO) {
        glDeleteBuffers(1, &m_quadVBO);
        m_quadVBO = 0;
    }

    m_mainShader.reset();
    m_shadowShader.reset();
    m_postProcessShader.reset();
    m_skyShader.reset();
    m_particleShader.reset();

    m_initialized = false;
    LOG_INFO("Renderer shut down");
}

void Renderer::beginFrame() {
    resetStats();
    clear();

    if (m_camera) {
        m_camera->update();
    }

    // Begin rendering to post-processing framebuffer
    if (m_settings.bloom) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_postFBO);
        glViewport(0, 0, m_width, m_height);
        clear();
    }
}

void Renderer::endFrame() {
    if (m_settings.bloom) {
        renderPostProcessing();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
}

void Renderer::present() {
    // Swap is handled by the window
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::setCamera(Camera* camera) {
    m_camera = camera;
}

void Renderer::submit(const RenderCommand& command) {
    m_commandQueue.push_back(command);
}

void Renderer::flush() {
    if (!m_camera || !m_mainShader) return;

    m_mainShader->bind();
    m_mainShader->setMat4("view", m_camera->getViewMatrix());
    m_mainShader->setMat4("projection", m_camera->getProjectionMatrix());
    m_mainShader->setVec3("viewPos", m_camera->getPosition());

    // Set fog
    if (m_settings.fog) {
        m_mainShader->setBool("fogEnabled", true);
        m_mainShader->setVec3("fogColor", m_settings.fogColor);
        m_mainShader->setFloat("fogNear", m_settings.fogNear);
        m_mainShader->setFloat("fogFar", m_settings.fogFar);
    } else {
        m_mainShader->setBool("fogEnabled", false);
    }

    // Set ambient light
    m_mainShader->setVec3("ambientColor", m_ambientColor * m_ambientIntensity);

    // Set lights
    int numLights = glm::min(static_cast<int>(m_lights.size()), 16);
    m_mainShader->setInt("numLights", numLights);

    for (int i = 0; i < numLights; i++) {
        std::string prefix = "lights[" + std::to_string(i) + "].";
        const Light& light = m_lights[i];

        m_mainShader->setInt(prefix + "type", static_cast<int>(light.type));
        m_mainShader->setVec3(prefix + "position", light.position);
        m_mainShader->setVec3(prefix + "direction", light.direction);
        m_mainShader->setVec3(prefix + "color", light.color * light.intensity);
        m_mainShader->setFloat(prefix + "range", light.range);
        m_mainShader->setFloat(prefix + "constant", light.constant);
        m_mainShader->setFloat(prefix + "linear", light.linear);
        m_mainShader->setFloat(prefix + "quadratic", light.quadratic);
        m_mainShader->setFloat(prefix + "innerCutoff", glm::cos(glm::radians(light.innerConeAngle)));
        m_mainShader->setFloat(prefix + "outerCutoff", glm::cos(glm::radians(light.outerConeAngle)));
    }

    // Sort commands by shader and texture for better batching
    std::sort(m_commandQueue.begin(), m_commandQueue.end(),
        [](const RenderCommand& a, const RenderCommand& b) {
            if (a.shader != b.shader) return a.shader < b.shader;
            return a.textureID < b.textureID;
        });

    GLuint lastShader = 0;
    GLuint lastTexture = 0;

    for (const auto& cmd : m_commandQueue) {
        // Bind shader if different
        if (cmd.shader && cmd.shader->getID() != lastShader) {
            if (lastShader != 0) {
                m_mainShader->bind();
            }
            lastShader = cmd.shader ? cmd.shader->getID() : 0;
            m_stats.shaderBinds++;
        }

        // Bind texture if different
        if (cmd.textureID != lastTexture && cmd.textureID != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, cmd.textureID);
            lastTexture = cmd.textureID;
            m_stats.textureBindings++;
        }

        // Set model matrix
        m_mainShader->setMat4("model", cmd.modelMatrix);

        // Draw
        glBindVertexArray(cmd.vao);

        if (cmd.indexed) {
            glDrawElements(GL_TRIANGLES, cmd.indexCount, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, cmd.indexCount);
        }

        m_stats.drawCalls++;
        m_stats.triangles += cmd.indexCount / 3;
    }

    m_commandQueue.clear();
    glBindVertexArray(0);
}

void Renderer::setAmbientLight(const glm::vec3& color, float intensity) {
    m_ambientColor = color;
    m_ambientIntensity = intensity;
}

void Renderer::addLight(const Light& light) {
    if (m_lights.size() < 16) {
        m_lights.push_back(light);
    }
}

void Renderer::removeLight(int index) {
    if (index >= 0 && index < static_cast<int>(m_lights.size())) {
        m_lights.erase(m_lights.begin() + index);
    }
}

void Renderer::clearLights() {
    m_lights.clear();
}

void Renderer::enablePostProcessing(bool enabled) {
    m_settings.bloom = enabled;
}

void Renderer::setPostProcessingParams(float bloom, float vignette, float saturation) {
    m_settings.bloomIntensity = bloom;
    // vignette and saturation would be used in post-process shader
}

void Renderer::applySettings(const RenderSettings& settings) {
    m_settings = settings;

    glClearColor(settings.clearColor.r, settings.clearColor.g,
                 settings.clearColor.b, settings.clearColor.a);

    if (settings.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (settings.faceCulling) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }

    if (settings.depthTest) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::resetStats() {
    m_stats.drawCalls = 0;
    m_stats.vertices = 0;
    m_stats.triangles = 0;
    m_stats.textureBindings = 0;
    m_stats.shaderBinds = 0;
}

void Renderer::setupDefaultShaders() {
    // Main shader
    m_mainShader = std::make_unique<ShaderProgram>();

    Shader vertexShader;
    Shader fragmentShader;

    // These would normally be loaded from files
    // For now, we'll use embedded shaders
    const char* mainVertexSource = R"(
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;
out float FogFactor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
uniform bool fogEnabled;
uniform float fogNear;
uniform float fogFar;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalize(normalMatrix * aNormal);
    Tangent = normalize(normalMatrix * aTangent);
    Bitangent = normalize(normalMatrix * aBitangent);
    
    TexCoords = aTexCoords;
    
    vec4 viewPos4 = view * worldPos;
    float dist = -viewPos4.z;
    
    if (fogEnabled) {
        FogFactor = clamp((fogFar - dist) / (fogFar - fogNear), 0.0, 1.0);
    } else {
        FogFactor = 1.0;
    }
    
    gl_Position = projection * viewPos4;
}
)";

    const char* mainFragmentSource = R"(
#version 450 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;
in vec3 Bitangent;
in float FogFactor;

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float range;
    float constant;
    float linear;
    float quadratic;
    float innerCutoff;
    float outerCutoff;
};

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform int numLights;
uniform Light lights[16];
uniform vec3 fogColor;
uniform bool fogEnabled;

vec3 calculateLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 result = vec3(0.0);
    
    if (light.type == 0) { // Directional
        vec3 lightDir = normalize(-light.direction);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        result = light.color * (diff + spec);
    }
    else if (light.type == 1) { // Point
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
                                    light.quadratic * distance * distance);
        result = light.color * (diff + spec) * attenuation;
    }
    else if (light.type == 2) { // Spot
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.innerCutoff - light.outerCutoff;
        float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
        
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + 
                                    light.quadratic * distance * distance);
        result = light.color * (diff + spec) * intensity * attenuation;
    }
    
    return result;
}

void main() {
    vec3 color = texture(diffuseMap, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 lighting = ambientColor;
    
    for (int i = 0; i < numLights; i++) {
        lighting += calculateLight(lights[i], normal, FragPos, viewDir);
    }
    
    vec3 finalColor = color * lighting;
    
    if (fogEnabled) {
        finalColor = mix(fogColor, finalColor, FogFactor);
    }
    
    FragColor = vec4(finalColor, 1.0);
}
)";

    vertexShader.loadFromSource(mainVertexSource, ShaderType::VERTEX);
    fragmentShader.loadFromSource(mainFragmentSource, ShaderType::FRAGMENT);

    m_mainShader->attachShader(vertexShader);
    m_mainShader->attachShader(fragmentShader);
    m_mainShader->link();
}

void Renderer::setupPostProcessing() {
    // Create framebuffer for post-processing
    glGenFramebuffers(1, &m_postFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_postFBO);

    // Create texture for color attachment
    glGenTextures(1, &m_postTexture);
    glBindTexture(GL_TEXTURE_2D, m_postTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_postTexture, 0);

    // Create depth buffer
    glGenRenderbuffers(1, &m_postDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_postDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_postDepthBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_ERROR("Post-processing framebuffer incomplete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create quad for post-processing
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Post-process shader
    m_postProcessShader = std::make_unique<ShaderProgram>();

    Shader ppVert, ppFrag;

    const char* ppVertexSource = R"(
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
void main() {
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
)";

    const char* ppFragmentSource = R"(
#version 450 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;
uniform float bloomIntensity;
uniform float vignetteIntensity;

void main() {
    vec3 color = texture(screenTexture, TexCoords).rgb;
    
    // Vignette
    vec2 center = TexCoords - 0.5;
    float vignette = 1.0 - dot(center, center) * vignetteIntensity;
    color *= vignette;
    
    FragColor = vec4(color, 1.0);
}
)";

    ppVert.loadFromSource(ppVertexSource, ShaderType::VERTEX);
    ppFrag.loadFromSource(ppFragmentSource, ShaderType::FRAGMENT);

    m_postProcessShader->attachShader(ppVert);
    m_postProcessShader->attachShader(ppFrag);
    m_postProcessShader->link();
}

void Renderer::renderPostProcessing() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_postProcessShader->bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_postTexture);
    m_postProcessShader->setInt("screenTexture", 0);
    m_postProcessShader->setFloat("bloomIntensity", m_settings.bloomIntensity);
    m_postProcessShader->setFloat("vignetteIntensity", 0.5f);

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::drawQuad() {
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::drawCube() {
    // TODO: Implement cube drawing
    static GLuint cubeVAO = 0;
    static GLuint cubeVBO = 0;

    if (cubeVAO == 0) {
        float vertices[] = {
            // Front face
            -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
            // Back face
            -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
            // Left face
            -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,
            // Right face
             0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,
            // Top face
            -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,
            // Bottom face
            -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_QUADS, 0, 24);
    glBindVertexArray(0);
}

void Renderer::drawSphere(int segments) {
    // TODO: Implement sphere drawing
}

} // namespace ExperimentRedbear
