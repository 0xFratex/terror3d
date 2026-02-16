#pragma once

#include <unordered_map>
#include <unordered_set>
#include <glm/glm.hpp>

namespace ExperimentRedbear {

class Input {
public:
    static Input& getInstance();

    void initialize();
    void update();

    // Keyboard
    bool isKeyDown(int key) const;
    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;

    // Mouse
    bool isMouseButtonDown(int button) const;
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonReleased(int button) const;

    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;
    float getMouseScroll() const;

    void setMousePosition(const glm::vec2& pos);
    void setCursorMode(int mode); // GLFW_CURSOR_NORMAL, GLFW_CURSOR_DISABLED, etc.

    // Internal callbacks
    void keyCallback(int key, int scancode, int action, int mods);
    void mouseCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);
    void scrollCallback(double xoffset, double yoffset);

private:
    Input() = default;
    ~Input() = default;
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    std::unordered_set<int> m_keysDown;
    std::unordered_set<int> m_keysPressed;
    std::unordered_set<int> m_keysReleased;

    std::unordered_set<int> m_mouseButtonsDown;
    std::unordered_set<int> m_mouseButtonsPressed;
    std::unordered_set<int> m_mouseButtonsReleased;

    glm::vec2 m_mousePosition = glm::vec2(0.0f);
    glm::vec2 m_lastMousePosition = glm::vec2(0.0f);
    glm::vec2 m_mouseDelta = glm::vec2(0.0f);
    float m_mouseScroll = 0.0f;

    bool m_firstMouse = true;
};

} // namespace ExperimentRedbear
