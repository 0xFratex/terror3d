#pragma once

#include "UIElement.h"
#include <vector>
#include <memory>

namespace ExperimentRedbear {

class Panel : public UIElement, public std::enable_shared_from_this<Panel> {
public:
    Panel(const std::string& id = "");
    ~Panel() override = default;

    // Background
    void setBackgroundColor(const glm::vec4& color) { m_backgroundColor = color; }
    const glm::vec4& getBackgroundColor() const { return m_backgroundColor; }

    void setBackgroundImage(const std::string& texturePath) { m_backgroundImage = texturePath; }
    const std::string& getBackgroundImage() const { return m_backgroundImage; }

    // Border
    void setBorderColor(const glm::vec4& color) { m_borderColor = color; }
    void setBorderWidth(float width) { m_borderWidth = width; }

    // Children
    void addChild(std::shared_ptr<UIElement> child);
    void removeChild(std::shared_ptr<UIElement> child);
    const std::vector<std::shared_ptr<UIElement>>& getChildren() const { return m_children; }

    // Overrides
    void update(float deltaTime) override;
    void render() override;
    bool containsPoint(const glm::vec2& point) const override;

private:
    glm::vec4 m_backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 0.8f);
    std::string m_backgroundImage;
    glm::vec4 m_borderColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    float m_borderWidth = 1.0f;

    std::vector<std::shared_ptr<UIElement>> m_children;
};

} // namespace ExperimentRedbear
