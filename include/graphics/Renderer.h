#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "graphics/Camera.h"
#include "graphics/Light.h"
#include "graphics/Shader.h"

namespace ExperimentRedbear {

struct RenderStats {
    int drawCalls = 0;
    int vertices = 0;
    int triangles = 0;
    int textureBindings = 0;
    int shaderBinds = 0;
    float cpuTime = 0.0f;
    float gpuTime = 0.0f;
};

struct RenderSettings {
    bool wireframe = false;
    bool faceCulling = true;
    bool depthTest = true;
    bool blend = true;
    glm::vec4 clearColor = glm::vec4(0.02f, 0.02f, 0.03f, 1.0f);  // Very dark blue
    bool fog = true;
    glm::vec3 fogColor = glm::vec3(0.1f, 0.1f, 0.12f);
    float fogNear = 10.0f;
    float fogFar = 100.0f;
    bool bloom = false;  // Disabled by default
    float bloomIntensity = 0.5f;
    bool ssao = true;
    float ssaoRadius = 0.5f;
};

struct RenderCommand {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint textureID;
    ShaderProgram* shader;
    glm::mat4 modelMatrix;
    int indexCount;
    bool indexed;
};

class Renderer {
public:
    static Renderer& getInstance();

    bool initialize(int width, int height);
    void shutdown();

    void beginFrame();
    void endFrame();
    void present();

    void clear();
    void setViewport(int x, int y, int width, int height);

    void setCamera(Camera* camera);
    Camera* getCamera() const { return m_camera; }

    void submit(const RenderCommand& command);
    void flush();

    // Lighting
    void setAmbientLight(const glm::vec3& color, float intensity);
    void addLight(const Light& light);
    void removeLight(int index);
    void clearLights();
    const std::vector<Light>& getLights() const { return m_lights; }

    // Post-processing
    void enablePostProcessing(bool enabled);
    void setPostProcessingParams(float bloom, float vignette, float saturation);

    // Settings
    RenderSettings& getSettings() { return m_settings; }
    void applySettings(const RenderSettings& settings);

    // Stats
    const RenderStats& getStats() const { return m_stats; }
    void resetStats();

    // Utility
    void drawQuad();
    void drawCube();
    void drawSphere(int segments = 16);

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void setupDefaultShaders();
    void setupPostProcessing();
    void renderPostProcessing();

    int m_width = 0;
    int m_height = 0;

    Camera* m_camera = nullptr;

    std::vector<RenderCommand> m_commandQueue;
    std::vector<Light> m_lights;
    glm::vec3 m_ambientColor = glm::vec3(0.02f);
    float m_ambientIntensity = 1.0f;

    RenderSettings m_settings;
    RenderStats m_stats;

    // Shaders
    std::unique_ptr<ShaderProgram> m_mainShader;
    std::unique_ptr<ShaderProgram> m_shadowShader;
    std::unique_ptr<ShaderProgram> m_postProcessShader;
    std::unique_ptr<ShaderProgram> m_skyShader;
    std::unique_ptr<ShaderProgram> m_particleShader;

    // Post-processing
    GLuint m_postFBO = 0;
    GLuint m_postTexture = 0;
    GLuint m_postDepthBuffer = 0;
    GLuint m_quadVAO = 0;
    GLuint m_quadVBO = 0;

    bool m_initialized = false;
};

} // namespace ExperimentRedbear
