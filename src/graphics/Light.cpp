#include "graphics/Light.h"

namespace ExperimentRedbear {

Light Light::createDirectional(const glm::vec3& direction, const glm::vec3& color, float intensity) {
    Light light;
    light.type = LightType::DIRECTIONAL;
    light.direction = direction;
    light.color = color;
    light.intensity = intensity;
    return light;
}

Light Light::createPoint(const glm::vec3& position, const glm::vec3& color, float intensity, float range) {
    Light light;
    light.type = LightType::POINT;
    light.position = position;
    light.color = color;
    light.intensity = intensity;
    light.range = range;
    return light;
}

Light Light::createSpot(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& color,
                         float intensity, float innerAngle, float outerAngle, float range) {
    Light light;
    light.type = LightType::SPOT;
    light.position = position;
    light.direction = direction;
    light.color = color;
    light.intensity = intensity;
    light.innerConeAngle = innerAngle;
    light.outerConeAngle = outerAngle;
    light.range = range;
    return light;
}

} // namespace ExperimentRedbear
