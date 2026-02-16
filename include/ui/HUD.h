#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace ExperimentRedbear {

class Player;

class HUD {
public:
    HUD();
    ~HUD();

    bool initialize();
    void update(float deltaTime);
    void render(const Player& player);

    // Health display
    void showHealthWarning();
    void setHealthFlash(float intensity);

    // Interaction prompt
    void showInteractionPrompt(const std::string& text);
    void hideInteractionPrompt();

    // Objectives
    void showObjective(const std::string& objective);
    void hideObjective();

    // Notifications
    void showNotification(const std::string& text, float duration = 3.0f);

    // Item pickup
    void showItemPickup(const std::string& itemName);

    // Effects
    void triggerDamageEffect(float intensity);
    void triggerHealEffect();
    void triggerFearEffect(float intensity);

    // Crosshair
    void setCrosshairVisible(bool visible);
    void setCrosshairType(int type); // 0=dot, 1=cross, 2=circle

    // Flashlight indicator
    void setFlashlightBattery(float level);

    // Debug info
    void setDebugInfo(const std::string& info);
    void setShowDebugInfo(bool show) { m_showDebugInfo = show; }

private:
    void renderHealthBar(const Player& player);
    void renderStaminaBar(const Player& player);
    void renderCrosshair();
    void renderInteractionPrompt();
    void renderObjectives();
    void renderNotification();
    void renderItemPickup();
    void renderFlashlightIndicator();
    void renderDebugInfo();
    void renderDamageEffect();
    void renderVignette();

    // State
    std::string m_interactionText;
    bool m_showInteraction = false;

    std::string m_currentObjective;
    bool m_showObjective = false;

    std::string m_notificationText;
    float m_notificationTimer = 0.0f;

    std::string m_itemPickupText;
    float m_itemPickupTimer = 0.0f;

    // Effects
    float m_damageEffectIntensity = 0.0f;
    float m_healEffectIntensity = 0.0f;
    float m_fearEffectIntensity = 0.0f;
    float m_vignetteIntensity = 0.3f;

    // Crosshair
    bool m_crosshairVisible = true;
    int m_crosshairType = 0;

    // Flashlight
    float m_flashlightBattery = 100.0f;

    // Debug
    std::string m_debugInfo;
    bool m_showDebugInfo = false;

    // Animation
    float m_pulseTime = 0.0f;
    float m_healthWarningPulse = 0.0f;
};

} // namespace ExperimentRedbear
