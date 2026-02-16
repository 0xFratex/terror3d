#include "game/Flashlight.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace ExperimentRedbear {

Flashlight::Flashlight() {
    m_light = Light::createSpot(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        m_color,
        m_baseIntensity,
        12.5f, 20.0f, 25.0f
    );
}

Flashlight::~Flashlight() = default;

void Flashlight::initialize() {
    m_enabled = false;
    m_batteryLevel = m_maxBattery;
    m_light.castShadows = true;
    m_light.shadowMapResolution = 512;
}

void Flashlight::update(float deltaTime, const glm::vec3& position, const glm::vec3& direction) {
    if (!m_enabled) return;

    // Update light position and direction
    m_light.position = position;
    m_light.direction = direction;

    // Update battery
    updateBattery(deltaTime);

    // Update flicker
    updateFlicker(deltaTime);

    // Adjust intensity based on battery level
    float batteryFactor = m_batteryLevel / m_maxBattery;
    m_light.intensity = m_baseIntensity * batteryFactor;
}

void Flashlight::updateBattery(float deltaTime) {
    if (m_enabled && m_batteryLevel > 0.0f) {
        m_batteryLevel -= m_batteryDrainRate * deltaTime;
        m_batteryLevel = std::max(0.0f, m_batteryLevel);

        if (m_batteryLevel <= 0.0f) {
            m_enabled = false;
            m_light.intensity = 0.0f;
        }

        // Enable flicker when battery is low
        if (m_batteryLevel < 20.0f) {
            m_flickerEnabled = true;
        }
    }
}

void Flashlight::updateFlicker(float deltaTime) {
    if (!m_flickerEnabled) {
        m_light.intensity = m_baseIntensity;
        return;
    }

    m_flickerTimer -= deltaTime;

    if (m_flickerTimer <= 0.0f) {
        // Random flicker
        float flickerChance = static_cast<float>(rand()) / RAND_MAX;
        if (flickerChance > 0.7f) {
            m_light.intensity = m_baseIntensity * (0.3f + static_cast<float>(rand()) / RAND_MAX * 0.7f);
        } else {
            m_light.intensity = m_baseIntensity;
        }
        m_flickerTimer = 0.05f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
    }
}

void Flashlight::toggle() {
    if (m_batteryLevel > 0.0f) {
        m_enabled = !m_enabled;
    }
}

void Flashlight::setEnabled(bool enabled) {
    if (enabled && m_batteryLevel > 0.0f) {
        m_enabled = true;
    } else {
        m_enabled = false;
    }
}

void Flashlight::setBatteryLevel(float level) {
    m_batteryLevel = std::clamp(level, 0.0f, m_maxBattery);
}

void Flashlight::recharge(float amount) {
    m_batteryLevel = std::min(m_maxBattery, m_batteryLevel + amount);
    if (m_batteryLevel > 20.0f) {
        m_flickerEnabled = false;
    }
}

} // namespace ExperimentRedbear
