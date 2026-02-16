#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <glm/glm.hpp>

namespace ExperimentRedbear {

enum class MenuState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    SETTINGS,
    AUDIO_SETTINGS,
    GRAPHICS_SETTINGS,
    GAMEPLAY_SETTINGS,
    CREDITS,
    CONFIRM_QUIT
};

class Menu {
public:
    Menu();
    ~Menu();

    bool initialize();
    void update(float deltaTime);
    void render();

    MenuState getState() const { return m_state; }
    void setState(MenuState state);

    // Navigation
    void goToPreviousMenu();

    // Selection
    void selectNext();
    void selectPrevious();
    void selectCurrent();

    // Callbacks
    void setOnNewGame(std::function<void()> callback) { m_onNewGame = callback; }
    void setOnContinue(std::function<void()> callback) { m_onContinue = callback; }
    void setOnQuit(std::function<void()> callback) { m_onQuit = callback; }

    // Settings
    void applySettings();
    void resetSettings();

private:
    void renderMainMenu();
    void renderPauseMenu();
    void renderSettingsMenu();
    void renderAudioSettings();
    void renderGraphicsSettings();
    void renderGameplaySettings();
    void renderConfirmQuit();

    MenuState m_state = MenuState::MAIN_MENU;
    MenuState m_previousState = MenuState::MAIN_MENU;
    int m_selectedIndex = 0;

    // Menu items for current state
    std::vector<std::string> m_currentItems;

    // Styling
    glm::vec3 m_normalColor = glm::vec3(0.7f, 0.7f, 0.7f);
    glm::vec3 m_selectedColor = glm::vec3(0.9f, 0.2f, 0.2f);
    glm::vec3 m_disabledColor = glm::vec3(0.4f, 0.4f, 0.4f);
    glm::vec3 m_titleColor = glm::vec3(0.8f, 0.1f, 0.1f);

    // Callbacks
    std::function<void()> m_onNewGame;
    std::function<void()> m_onContinue;
    std::function<void()> m_onQuit;

    // Animation
    float m_animationTime = 0.0f;
    float m_transitionProgress = 1.0f;

    // Title effect
    float m_titleGlow = 0.0f;
};

} // namespace ExperimentRedbear
