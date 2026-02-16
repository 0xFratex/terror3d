#include "ui/Button.h"

namespace ExperimentRedbear {

Button::Button(const std::string& id)
    : UIElement(id) {
}

void Button::update(float deltaTime) {
    // Button update logic
}

void Button::render() {
    // Button rendering would be done here
    // For now, this is a placeholder
}

void Button::onMouseEnter() {
    m_hovered = true;
}

void Button::onMouseLeave() {
    m_hovered = false;
    m_pressed = false;
}

void Button::onMouseClick(int button, bool pressed) {
    if (m_disabled) return;

    if (pressed) {
        m_pressed = true;
    } else {
        if (m_pressed && m_hovered && m_onClick) {
            m_onClick();
        }
        m_pressed = false;
    }
}

} // namespace ExperimentRedbear
