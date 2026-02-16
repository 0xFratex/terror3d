#pragma once

#include "UIElement.h"
#include <functional>
#include <string>

namespace ExperimentRedbear {

class Slider : public UIElement {
public:
    Slider(const std::string& id = "");
    ~Slider() override = default;

    // Value
    float getValue() const { return m_value; }
    void setValue(float value);
    float getMinValue() const { return m_minValue; }
    void setMinValue(float min) { m_minValue = min; }
    float getMaxValue() const { return m_maxValue; }
    void setMaxValue(float max) { m_maxValue = max; }
    void setRange(float min, float max);

    // Appearance
    void setTrackColor(const glm::vec4& color) { m_trackColor = color; }
    void setFillColor(const glm::vec4& color) { m_fillColor = color; }
    void setHandleColor(const glm::vec4& color) { m_handleColor = color; }
    void setHandleSize(const glm::vec2& size) { m_handleSize = size; }

    // Callback
    using ValueChangedCallback = std::function<void(float)>;
    void setOnValueChanged(ValueChangedCallback callback) { m_onValueChanged = callback; }

    // Overrides
    void update(float deltaTime) override;
    void render() override;
    void onMouseMove(const glm::vec2& pos) override;
    void onMouseClick(int button, bool pressed) override;

private:
    float m_value = 0.5f;
    float m_minValue = 0.0f;
    float m_maxValue = 1.0f;

    glm::vec4 m_trackColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 m_fillColor = glm::vec4(0.4f, 0.6f, 0.8f, 1.0f);
    glm::vec4 m_handleColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    glm::vec2 m_handleSize = glm::vec2(20.0f, 30.0f);

    bool m_dragging = false;

    ValueChangedCallback m_onValueChanged;

    void updateValueFromPosition(float x);
};

} // namespace ExperimentRedbear
