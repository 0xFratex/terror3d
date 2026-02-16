#include "ui/Slider.h"
#include <algorithm>

namespace ExperimentRedbear {

Slider::Slider(const std::string& id)
    : UIElement(id) {
}

void Slider::setValue(float value) {
    m_value = std::clamp(value, m_minValue, m_maxValue);
    if (m_onValueChanged) {
        m_onValueChanged(m_value);
    }
}

void Slider::setRange(float min, float max) {
    m_minValue = min;
    m_maxValue = max;
    setValue(m_value); // Clamp current value
}

void Slider::update(float deltaTime) {
    // Slider update logic
}

void Slider::render() {
    // Slider rendering would be done here
    // For now, this is a placeholder
}

void Slider::onMouseMove(const glm::vec2& pos) {
    if (m_dragging) {
        updateValueFromPosition(pos.x);
    }
}

void Slider::onMouseClick(int button, bool pressed) {
    if (pressed) {
        m_dragging = true;
        // Also update value on initial click
    } else {
        m_dragging = false;
    }
}

void Slider::updateValueFromPosition(float x) {
    float relativeX = x - m_position.x;
    float percent = relativeX / m_size.x;
    percent = std::clamp(percent, 0.0f, 1.0f);

    float newValue = m_minValue + percent * (m_maxValue - m_minValue);
    setValue(newValue);
}

} // namespace ExperimentRedbear
