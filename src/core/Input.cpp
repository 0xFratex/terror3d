#include "core/Input.h"
#include <GLFW/glfw3.h>

namespace ExperimentRedbear {

Input& Input::getInstance() {
    static Input instance;
    return instance;
}

void Input::initialize() {
    m_keysDown.clear();
    m_keysPressed.clear();
    m_keysReleased.clear();
    m_mouseButtonsDown.clear();
    m_mouseButtonsPressed.clear();
    m_mouseButtonsReleased.clear();
    m_firstMouse = true;
}

void Input::update() {
    m_keysPressed.clear();
    m_keysReleased.clear();
    m_mouseButtonsPressed.clear();
    m_mouseButtonsReleased.clear();
    m_mouseDelta = glm::vec2(0.0f);
    m_mouseScroll = 0.0f;
}

bool Input::isKeyDown(int key) const {
    return m_keysDown.find(key) != m_keysDown.end();
}

bool Input::isKeyPressed(int key) const {
    return m_keysPressed.find(key) != m_keysPressed.end();
}

bool Input::isKeyReleased(int key) const {
    return m_keysReleased.find(key) != m_keysReleased.end();
}

bool Input::isMouseButtonDown(int button) const {
    return m_mouseButtonsDown.find(button) != m_mouseButtonsDown.end();
}

bool Input::isMouseButtonPressed(int button) const {
    return m_mouseButtonsPressed.find(button) != m_mouseButtonsPressed.end();
}

bool Input::isMouseButtonReleased(int button) const {
    return m_mouseButtonsReleased.find(button) != m_mouseButtonsReleased.end();
}

glm::vec2 Input::getMousePosition() const {
    return m_mousePosition;
}

glm::vec2 Input::getMouseDelta() const {
    return m_mouseDelta;
}

float Input::getMouseScroll() const {
    return m_mouseScroll;
}

void Input::setMousePosition(const glm::vec2& pos) {
    m_mousePosition = pos;
    m_lastMousePosition = pos;
}

void Input::setCursorMode(int mode) {
    // This would need access to the GLFW window
    // For now, we'll just track the mode
}

void Input::keyCallback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        m_keysDown.insert(key);
        m_keysPressed.insert(key);
    } else if (action == GLFW_RELEASE) {
        m_keysDown.erase(key);
        m_keysReleased.insert(key);
    }
}

void Input::mouseCallback(double xpos, double ypos) {
    m_mousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
    
    if (m_firstMouse) {
        m_lastMousePosition = m_mousePosition;
        m_firstMouse = false;
    }
    
    m_mouseDelta = m_mousePosition - m_lastMousePosition;
    m_lastMousePosition = m_mousePosition;
}

void Input::mouseButtonCallback(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        m_mouseButtonsDown.insert(button);
        m_mouseButtonsPressed.insert(button);
    } else if (action == GLFW_RELEASE) {
        m_mouseButtonsDown.erase(button);
        m_mouseButtonsReleased.insert(button);
    }
}

void Input::scrollCallback(double xoffset, double yoffset) {
    m_mouseScroll = static_cast<float>(yoffset);
}

} // namespace ExperimentRedbear
