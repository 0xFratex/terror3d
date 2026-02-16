#pragma once

#include <glm/glm.hpp>

namespace ExperimentRedbear {

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

struct Light {
    LightType type = LightType::POINT;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;

    // Attenuation
    float range = 10.0f;
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    // Spot light
    float innerConeAngle = 12.5f;
    float outerConeAngle = 17.5f;

    // Shadows
    bool castShadows = true;
    int shadowMapResolution = 1024;

    // Animation
    bool animated = false;
    float flickerSpeed = 1.0f;
    float flickerIntensity = 0.1f;

    static Light createDirectional(const glm::vec3& direction, const glm::vec3& color, float intensity);
    static Light createPoint(const glm::vec3& position, const glm::vec3& color, float intensity, float range);
    static Light createSpot(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color, 
                            float intensity, float innerAngle, float outerAngle, float range);
};

} // namespace ExperimentRedbear
