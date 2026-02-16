#include "ui/UIManager.h"
#include "ui/UIElement.h"
#include "core/Logger.h"
#include <algorithm>

namespace ExperimentRedbear {

UIManager& UIManager::getInstance() {
    static UIManager instance;
    return instance;
}

bool UIManager::initialize(int screenWidth, int screenHeight) {
    m_width = screenWidth;
    m_height = screenHeight;

    // Create VAO and VBO for UI rendering
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_initialized = true;
    LOG_INFO("UI Manager initialized: " + std::to_string(screenWidth) + "x" + std::to_string(screenHeight));

    return true;
}

void UIManager::shutdown() {
    m_elements.clear();

    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vbo) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    m_initialized = false;
}

void UIManager::update(float deltaTime) {
    for (auto& element : m_elements) {
        if (element->isActive()) {
            // Update element
        }
    }
}

void UIManager::render() {
    // Disable depth test for UI
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render all elements
    for (auto& element : m_elements) {
        if (element->isActive()) {
            // Render element
        }
    }

    glEnable(GL_DEPTH_TEST);
}

void UIManager::addElement(std::shared_ptr<UIElement> element) {
    m_elements.push_back(element);
}

void UIManager::removeElement(const std::string& id) {
    m_elements.erase(
        std::remove_if(m_elements.begin(), m_elements.end(),
            [&id](const std::shared_ptr<UIElement>& elem) {
                // Would need proper ID system
                return false;
            }),
        m_elements.end()
    );
}

std::shared_ptr<UIElement> UIManager::getElement(const std::string& id) {
    for (auto& elem : m_elements) {
        // Would need proper ID system
    }
    return nullptr;
}

void UIManager::clearElements() {
    m_elements.clear();
}

void UIManager::onMouseMove(const glm::vec2& pos) {
    m_cursorPos = pos;

    for (auto& element : m_elements) {
        if (element->isActive()) {
            // Check hover state
        }
    }
}

void UIManager::onMouseClick(int button, bool pressed) {
    for (auto& element : m_elements) {
        if (element->isActive()) {
            // Handle click
        }
    }
}

void UIManager::onKey(int key, bool pressed) {
    // Handle keyboard navigation
}

void UIManager::resize(int width, int height) {
    m_width = width;
    m_height = height;

    for (auto& element : m_elements) {
        // Update element positions/sizes if needed
    }
}

void UIManager::showCursor(bool show) {
    m_cursorVisible = show;
}

void UIManager::setFocusedElement(std::shared_ptr<UIElement> element) {
    m_focusedElement = element;
}

} // namespace ExperimentRedbear
