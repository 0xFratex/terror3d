#include "ui/Menu.h"
#include "ui/TextRenderer.h"
#include "ui/UIManager.h"
#include "core/Input.h"
#include "core/Logger.h"
#include <GL/glew.h>

namespace ExperimentRedbear {

Menu::Menu() {
    m_state = MenuState::MAIN_MENU;
}

Menu::~Menu() = default;

bool Menu::initialize() {
    // Initialize menu items for the current state
    setState(MenuState::MAIN_MENU);
    LOG_INFO("Menu system initialized");
    return true;
}

void Menu::update(float deltaTime) {
    m_animationTime += deltaTime;

    // Title glow effect
    m_titleGlow = (std::sin(m_animationTime * 2.0f) + 1.0f) * 0.5f;

    // Handle input based on state
    Input& input = Input::getInstance();

    switch (m_state) {
        case MenuState::MAIN_MENU:
        case MenuState::PAUSED:
            if (input.isKeyPressed(87)) { // W or Up
                m_selectedIndex = (m_selectedIndex - 1 + static_cast<int>(m_currentItems.size())) 
                                  % static_cast<int>(m_currentItems.size());
            }
            if (input.isKeyPressed(83)) { // S or Down
                m_selectedIndex = (m_selectedIndex + 1) % static_cast<int>(m_currentItems.size());
            }
            if (input.isKeyPressed(257)) { // Enter
                selectCurrent();
            }
            break;

        case MenuState::SETTINGS:
        case MenuState::AUDIO_SETTINGS:
        case MenuState::GRAPHICS_SETTINGS:
        case MenuState::GAMEPLAY_SETTINGS:
            // Handle settings navigation
            if (input.isKeyPressed(256)) { // Escape
                goToPreviousMenu();
            }
            break;

        default:
            break;
    }
}

void Menu::render() {
    switch (m_state) {
        case MenuState::MAIN_MENU:
            renderMainMenu();
            break;
        case MenuState::PAUSED:
            renderPauseMenu();
            break;
        case MenuState::SETTINGS:
            renderSettingsMenu();
            break;
        case MenuState::AUDIO_SETTINGS:
            renderAudioSettings();
            break;
        case MenuState::GRAPHICS_SETTINGS:
            renderGraphicsSettings();
            break;
        case MenuState::GAMEPLAY_SETTINGS:
            renderGameplaySettings();
            break;
        case MenuState::CONFIRM_QUIT:
            renderConfirmQuit();
            break;
        default:
            break;
    }
}

void Menu::setState(MenuState state) {
    m_previousState = m_state;
    m_state = state;
    m_selectedIndex = 0;

    // Set current menu items
    switch (state) {
        case MenuState::MAIN_MENU:
            m_currentItems = {"New Game", "Continue", "Settings", "Quit"};
            break;
        case MenuState::PAUSED:
            m_currentItems = {"Resume", "Settings", "Quit to Menu"};
            break;
        case MenuState::SETTINGS:
            m_currentItems = {"Audio", "Graphics", "Gameplay", "Back"};
            break;
        default:
            m_currentItems.clear();
            break;
    }
}

void Menu::goToPreviousMenu() {
    setState(m_previousState);
}

void Menu::selectNext() {
    m_selectedIndex = (m_selectedIndex + 1) % static_cast<int>(m_currentItems.size());
}

void Menu::selectPrevious() {
    m_selectedIndex = (m_selectedIndex - 1 + static_cast<int>(m_currentItems.size())) 
                      % static_cast<int>(m_currentItems.size());
}

void Menu::selectCurrent() {
    if (m_state == MenuState::MAIN_MENU) {
        switch (m_selectedIndex) {
            case 0: // New Game
                if (m_onNewGame) m_onNewGame();
                break;
            case 1: // Continue
                if (m_onContinue) m_onContinue();
                break;
            case 2: // Settings
                setState(MenuState::SETTINGS);
                break;
            case 3: // Quit
                setState(MenuState::CONFIRM_QUIT);
                break;
        }
    } else if (m_state == MenuState::PAUSED) {
        switch (m_selectedIndex) {
            case 0: // Resume
                setState(MenuState::PLAYING);
                break;
            case 1: // Settings
                setState(MenuState::SETTINGS);
                break;
            case 2: // Quit to Menu
                setState(MenuState::MAIN_MENU);
                break;
        }
    }
}

void Menu::renderMainMenu() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    // Draw title with glow effect
    glm::vec3 titleColor = glm::mix(
        m_titleColor,
        glm::vec3(1.0f, 0.3f, 0.3f),
        m_titleGlow * 0.3f
    );

    // Title
    std::string title = "EXPERIMENT REDBEAR";
    glm::vec2 titleSize = textRenderer.measureText(title, "title", 2.0f);
    textRenderer.renderText(
        title,
        glm::vec2((width - titleSize.x) / 2.0f, height * 0.7f),
        "title", 2.0f, titleColor, true
    );

    // Subtitle
    std::string subtitle = "A Horror Experience";
    glm::vec2 subSize = textRenderer.measureText(subtitle, "main", 1.0f);
    textRenderer.renderText(
        subtitle,
        glm::vec2((width - subSize.x) / 2.0f, height * 0.65f),
        "main", 1.0f, m_normalColor, true
    );

    // Menu items
    float startY = height * 0.5f;
    float itemSpacing = 50.0f;

    for (size_t i = 0; i < m_currentItems.size(); i++) {
        glm::vec3 color = (i == static_cast<size_t>(m_selectedIndex)) ? m_selectedColor : m_normalColor;
        glm::vec2 itemSize = textRenderer.measureText(m_currentItems[i], "main", 1.2f);
        textRenderer.renderText(
            m_currentItems[i],
            glm::vec2((width - itemSize.x) / 2.0f, startY + i * itemSpacing),
            "main", 1.2f, color, true
        );
    }

    // Version info
    textRenderer.renderText(
        "Version 1.0.0 - Press W/S to navigate, Enter to select",
        glm::vec2(10.0f, 30.0f),
        "main", 0.8f, m_disabledColor
    );
}

void Menu::renderPauseMenu() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    // Semi-transparent overlay
    // (Would render a dark overlay here)

    // Title
    std::string title = "PAUSED";
    glm::vec2 titleSize = textRenderer.measureText(title, "title", 1.5f);
    textRenderer.renderText(
        title,
        glm::vec2((width - titleSize.x) / 2.0f, height * 0.3f),
        "title", 1.5f, m_titleColor, true
    );

    // Menu items
    float startY = height * 0.45f;
    float itemSpacing = 50.0f;

    for (size_t i = 0; i < m_currentItems.size(); i++) {
        glm::vec3 color = (i == static_cast<size_t>(m_selectedIndex)) ? m_selectedColor : m_normalColor;
        glm::vec2 itemSize = textRenderer.measureText(m_currentItems[i], "main", 1.2f);
        textRenderer.renderText(
            m_currentItems[i],
            glm::vec2((width - itemSize.x) / 2.0f, startY + i * itemSpacing),
            "main", 1.2f, color, true
        );
    }
}

void Menu::renderSettingsMenu() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    // Title
    std::string title = "SETTINGS";
    glm::vec2 titleSize = textRenderer.measureText(title, "title", 1.5f);
    textRenderer.renderText(
        title,
        glm::vec2((width - titleSize.x) / 2.0f, height * 0.2f),
        "title", 1.5f, m_titleColor, true
    );

    // Settings categories
    float startY = height * 0.35f;
    float itemSpacing = 60.0f;

    for (size_t i = 0; i < m_currentItems.size(); i++) {
        glm::vec3 color = (i == static_cast<size_t>(m_selectedIndex)) ? m_selectedColor : m_normalColor;
        glm::vec2 itemSize = textRenderer.measureText(m_currentItems[i], "main", 1.2f);
        textRenderer.renderText(
            m_currentItems[i],
            glm::vec2((width - itemSize.x) / 2.0f, startY + i * itemSpacing),
            "main", 1.2f, color, true
        );
    }
}

void Menu::renderAudioSettings() {
    // Audio settings sliders would go here
}

void Menu::renderGraphicsSettings() {
    // Graphics settings sliders would go here
}

void Menu::renderGameplaySettings() {
    // Gameplay settings sliders would go here
}

void Menu::renderConfirmQuit() {
    auto& textRenderer = TextRenderer::getInstance();
    auto& uiManager = UIManager::getInstance();

    int width = uiManager.getWidth();
    int height = uiManager.getHeight();

    std::string text = "Are you sure you want to quit?";
    glm::vec2 textSize = textRenderer.measureText(text, "main", 1.2f);
    textRenderer.renderText(
        text,
        glm::vec2((width - textSize.x) / 2.0f, height * 0.4f),
        "main", 1.2f, m_normalColor, true
    );

    if (m_selectedIndex == 0) {
        textRenderer.renderText("Yes", glm::vec2(width * 0.4f, height * 0.5f), "main", 1.2f, m_selectedColor);
        textRenderer.renderText("No", glm::vec2(width * 0.55f, height * 0.5f), "main", 1.2f, m_normalColor);
    } else {
        textRenderer.renderText("Yes", glm::vec2(width * 0.4f, height * 0.5f), "main", 1.2f, m_normalColor);
        textRenderer.renderText("No", glm::vec2(width * 0.55f, height * 0.5f), "main", 1.2f, m_selectedColor);
    }
}

void Menu::applySettings() {
    // Apply all changed settings
}

void Menu::resetSettings() {
    // Reset to default settings
}

} // namespace ExperimentRedbear
