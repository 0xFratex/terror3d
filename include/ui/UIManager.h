#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace ExperimentRedbear {

class UIElement;
class Panel;
class Button;
class Text;
class Image;

class UIManager {
public:
    static UIManager& getInstance();

    bool initialize(int screenWidth, int screenHeight);
    void shutdown();

    void update(float deltaTime);
    void render();

    // Element creation
    std::shared_ptr<Panel> createPanel(const std::string& id);
    std::shared_ptr<Button> createButton(const std::string& id);
    std::shared_ptr<Text> createText(const std::string& id);
    std::shared_ptr<Image> createImage(const std::string& id);

    // Element management
    void addElement(std::shared_ptr<UIElement> element);
    void removeElement(const std::string& id);
    std::shared_ptr<UIElement> getElement(const std::string& id);
    void clearElements();

    // Input handling
    void onMouseMove(const glm::vec2& pos);
    void onMouseClick(int button, bool pressed);
    void onKey(int key, bool pressed);

    // Screen
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    void resize(int width, int height);

    // Cursor
    void showCursor(bool show);
    bool isCursorVisible() const { return m_cursorVisible; }
    const glm::vec2& getCursorPosition() const { return m_cursorPos; }

    // Focused element
    void setFocusedElement(std::shared_ptr<UIElement> element);
    std::shared_ptr<UIElement> getFocusedElement() const { return m_focusedElement; }

private:
    UIManager() = default;
    ~UIManager() = default;
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    int m_width = 1920;
    int m_height = 1080;

    std::vector<std::shared_ptr<UIElement>> m_elements;
    std::shared_ptr<UIElement> m_focusedElement;

    glm::vec2 m_cursorPos = glm::vec2(0.0f);
    bool m_cursorVisible = true;
    bool m_initialized = false;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
};

} // namespace ExperimentRedbear
