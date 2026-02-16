#pragma once

#include "UIElement.h"
#include <functional>
#include <string>

namespace ExperimentRedbear {

class Button : public UIElement {
public:
    Button(const std::string& id = "");
    ~Button() override = default;

    // Text
    void setText(const std::string& text) { m_text = text; }
    const std::string& getText() const { return m_text; }

    // Colors
    void setNormalColor(const glm::vec4& color) { m_normalColor = color; }
    void setHoverColor(const glm::vec4& color) { m_hoverColor = color; }
    void setPressedColor(const glm::vec4& color) { m_pressedColor = color; }
    void setDisabledColor(const glm::vec4& color) { m_disabledColor = color; }

    // State
    bool isHovered() const { return m_hovered; }
    bool isPressed() const { return m_pressed; }
    bool isDisabled() const { return m_disabled; }
    void setDisabled(bool disabled) { m_disabled = disabled; }

    // Callback
    using ClickCallback = std::function<void()>;
    void setOnClick(ClickCallback callback) { m_onClick = callback; }

    // Overrides
    void update(float deltaTime) override;
    void render() override;
    void onMouseEnter() override;
    void onMouseLeave() override;
    void onMouseClick(int button, bool pressed) override;

private:
    std::string m_text;
    glm::vec4 m_normalColor = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
    glm::vec4 m_hoverColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
    glm::vec4 m_pressedColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 m_disabledColor = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);

    bool m_hovered = false;
    bool m_pressed = false;
    bool m_disabled = false;

    ClickCallback m_onClick;
};

} // namespace ExperimentRedbear
