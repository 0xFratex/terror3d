#pragma once

#include <glm/glm.hpp>
#include "graphics/Light.h"

namespace ExperimentRedbear {

class Flashlight {
public:
    Flashlight();
    ~Flashlight();

    void initialize();
    void update(float deltaTime, const glm::vec3& position, const glm::vec3& direction);

    void toggle();
    void setEnabled(bool enabled);
    bool isEnabled() const { return m_enabled; }

    const Light& getLight() const { return m_light; }
    Light& getLight() { return m_light; }

    // Battery system
    void setBatteryLevel(float level);
    float getBatteryLevel() const { return m_batteryLevel; }
    bool hasBattery() const { return m_batteryLevel > 0.0f; }
    void recharge(float amount);

    // Flicker effect
    void setFlickerEnabled(bool enabled) { m_flickerEnabled = enabled; }
    bool isFlickerEnabled() const { return m_flickerEnabled; }

private:
    void updateFlicker(float deltaTime);
    void updateBattery(float deltaTime);

    Light m_light;
    bool m_enabled = false;

    // Battery
    float m_batteryLevel = 100.0f;
    float m_maxBattery = 100.0f;
    float m_batteryDrainRate = 2.0f; // per second

    // Flicker
    bool m_flickerEnabled = false;
    float m_flickerTimer = 0.0f;
    float m_flickerDuration = 0.0f;
    float m_baseIntensity = 2.0f;

    // Color temperature (warm white)
    glm::vec3 m_color = glm::vec3(1.0f, 0.95f, 0.85f);
};

} // namespace ExperimentRedbear
