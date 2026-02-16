#include "ui/HUD.h"
#include "ui/TextRenderer.h"
#include "ui/UIManager.h"
#include "game/Player.h"
#include "core/Logger.h"
#include <GL/glew.h>
#include <cmath>

namespace ExperimentRedbear {

HUD::HUD() {}

HUD::~HUD() = default;

bool HUD::initialize() {
    LOG_INFO("HUD initialized");
    return true;
}

void HUD::update(float deltaTime) {
    m_pulseTime += deltaTime;

    // Update notification timer
    if (m_notificationTimer > 0.0f) {
        m_notificationTimer -= deltaTime;
        if (m_notificationTimer <= 0.0f) {
            m_notificationText.clear();
        }
    }

    // Update item pickup timer
    if (m_itemPickupTimer > 0.0f) {
        m_itemPickupTimer -= deltaTime;
        if (m_itemPickupTimer <= 0.0f) {
            m_itemPickupText.clear();
        }
    }

    // Update health warning pulse
    if (m_damageEffectIntensity > 0.0f) {
        m_damageEffectIntensity -= deltaTime * 2.0f;
        m_damageEffectIntensity = std::max(0.0f, m_damageEffectIntensity);
    }
}

void HUD::render(const Player& player) {
    auto& uiManager = UIManager::getInstance();

    // Render crosshair
    renderCrosshair();

    // Render health/stamina bars
    renderHealthBar(player);
    renderStaminaBar(player);

    // Render flashlight indicator
    renderFlashlightIndicator();

    // Render interaction prompt
    if (m_showInteraction) {
        renderInteractionPrompt();
    }

    // Render objectives
    if (m_showObjective) {
        renderObjectives();
    }

    // Render notification
    if (!m_notificationText.empty()) {
        renderNotification();
    }

    // Render item pickup
    if (!m_itemPickupText.empty()) {
        renderItemPickup();
    }

    // Render vignette effect
    renderVignette();

    // Render damage effect
    if (m_damageEffectIntensity > 0.0f) {
        renderDamageEffect();
    }

    // Render debug info
    if (m_showDebugInfo) {
        renderDebugInfo();
    }
}

void HUD::renderHealthBar(const Player& player) {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    const PlayerStats& stats = player.getStats();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    // Position
    float barWidth = 200.0f;
    float barHeight = 20.0f;
    float x = 30.0f;
    float y = height - 60.0f;

    // Background
    // (Would render a dark background quad)

    // Health fill
    float healthPercent = stats.health / stats.maxHealth;
    glm::vec3 healthColor = glm::vec3(1.0f, 0.2f, 0.2f);
    if (healthPercent > 0.6f) {
        healthColor = glm::vec3(0.2f, 0.8f, 0.2f);
    } else if (healthPercent > 0.3f) {
        healthColor = glm::vec3(0.8f, 0.8f, 0.2f);
    }

    // Low health warning pulse
    if (healthPercent < 0.3f) {
        m_healthWarningPulse = (std::sin(m_pulseTime * 4.0f) + 1.0f) * 0.5f;
        healthColor = glm::mix(healthColor, glm::vec3(1.0f, 0.0f, 0.0f), m_healthWarningPulse * 0.5f);
    }

    // Label
    textRenderer.renderText("HEALTH", glm::vec2(x, y - 20.0f), "main", 0.6f, glm::vec3(0.7f));
}

void HUD::renderStaminaBar(const Player& player) {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    const PlayerStats& stats = player.getStats();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    float barWidth = 150.0f;
    float barHeight = 10.0f;
    float x = 30.0f;
    float y = height - 100.0f;

    float staminaPercent = stats.stamina / stats.maxStamina;
    glm::vec3 staminaColor = glm::vec3(0.3f, 0.6f, 1.0f);

    // Label
    textRenderer.renderText("STAMINA", glm::vec2(x, y - 15.0f), "main", 0.5f, glm::vec3(0.6f));
}

void HUD::renderCrosshair() {
    if (!m_crosshairVisible) return;

    auto& uiManager = UIManager::getInstance();
    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    float centerX = width / 2.0f;
    float centerY = height / 2.0f;

    // Simple dot crosshair
    // (Would render a small white dot or cross)

    switch (m_crosshairType) {
        case 0: // Dot
            // Render a small filled circle
            break;
        case 1: // Cross
            // Render two intersecting lines
            break;
        case 2: // Circle
            // Render a small circle outline
            break;
    }
}

void HUD::renderInteractionPrompt() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    glm::vec2 textSize = textRenderer.measureText(m_interactionText, "main", 1.0f);
    textRenderer.renderText(
        m_interactionText,
        glm::vec2((width - textSize.x) / 2.0f, height / 2.0f + 50.0f),
        "main", 1.0f, glm::vec3(1.0f), true
    );
}

void HUD::renderObjectives() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();

    textRenderer.renderText(
        "OBJECTIVE",
        glm::vec2(width - 200.0f, 50.0f),
        "main", 0.7f, glm::vec3(0.8f)
    );

    textRenderer.renderText(
        m_currentObjective,
        glm::vec2(width - 200.0f, 75.0f),
        "main", 0.8f, glm::vec3(1.0f)
    );
}

void HUD::renderNotification() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    float alpha = m_notificationTimer > 0.5f ? 1.0f : m_notificationTimer * 2.0f;

    glm::vec2 textSize = textRenderer.measureText(m_notificationText, "main", 1.0f);
    textRenderer.renderText(
        m_notificationText,
        glm::vec2((width - textSize.x) / 2.0f, height * 0.7f),
        "main", 1.0f, glm::vec3(1.0f, 1.0f, 0.8f) * alpha, true
    );
}

void HUD::renderItemPickup() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    float alpha = m_itemPickupTimer > 1.0f ? 1.0f : m_itemPickupTimer;

    std::string text = "Picked up: " + m_itemPickupText;
    glm::vec2 textSize = textRenderer.measureText(text, "main", 0.9f);
    textRenderer.renderText(
        text,
        glm::vec2((width - textSize.x) / 2.0f, height * 0.65f),
        "main", 0.9f, glm::vec3(0.8f, 1.0f, 0.8f) * alpha, true
    );
}

void HUD::renderFlashlightIndicator() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    // Battery indicator
    float x = 30.0f;
    float y = height - 140.0f;

    // Battery icon and bar
    float batteryPercent = m_flashlightBattery / 100.0f;
    glm::vec3 batteryColor = glm::vec3(1.0f, 0.9f, 0.5f);
    if (batteryPercent < 0.2f) {
        batteryColor = glm::vec3(1.0f, 0.2f, 0.2f);
    } else if (batteryPercent < 0.4f) {
        batteryColor = glm::vec3(1.0f, 0.7f, 0.2f);
    }

    textRenderer.renderText(
        "FLASHLIGHT",
        glm::vec2(x, y - 15.0f),
        "main", 0.5f, glm::vec3(0.6f)
    );
}

void HUD::renderDebugInfo() {
    auto& textRenderer = TextRenderer::getInstance();

    textRenderer.renderText(
        m_debugInfo,
        glm::vec2(10.0f, 10.0f),
        "mono", 0.7f, glm::vec3(0.8f, 0.8f, 0.8f)
    );
}

void HUD::renderDamageEffect() {
    // Render red vignette overlay
    // (Would render a red semi-transparent overlay around the edges)
}

void HUD::renderVignette() {
    // Render subtle vignette effect
    // (Would render a dark gradient around the edges)
}

void HUD::showInteractionPrompt(const std::string& text) {
    m_interactionText = text;
    m_showInteraction = true;
}

void HUD::hideInteractionPrompt() {
    m_showInteraction = false;
}

void HUD::showObjective(const std::string& objective) {
    m_currentObjective = objective;
    m_showObjective = true;
}

void HUD::hideObjective() {
    m_showObjective = false;
}

void HUD::showNotification(const std::string& text, float duration) {
    m_notificationText = text;
    m_notificationTimer = duration;
}

void HUD::showItemPickup(const std::string& itemName) {
    m_itemPickupText = itemName;
    m_itemPickupTimer = 2.0f;
}

void HUD::triggerDamageEffect(float intensity) {
    m_damageEffectIntensity = std::min(1.0f, m_damageEffectIntensity + intensity);
}

void HUD::triggerHealEffect() {
    // Green flash or similar
}

void HUD::triggerFearEffect(float intensity) {
    m_fearEffectIntensity = intensity;
    m_vignetteIntensity = 0.3f + intensity * 0.3f;
}

void HUD::setCrosshairVisible(bool visible) {
    m_crosshairVisible = visible;
}

void HUD::setCrosshairType(int type) {
    m_crosshairType = type;
}

void HUD::setFlashlightBattery(float level) {
    m_flashlightBattery = level;
}

void HUD::setDebugInfo(const std::string& info) {
    m_debugInfo = info;
}

void HUD::showHealthWarning() {
    m_healthWarningPulse = 1.0f;
}

void HUD::setHealthFlash(float intensity) {
    m_damageEffectIntensity = intensity;
}

} // namespace ExperimentRedbear
