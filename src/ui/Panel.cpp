#include "ui/Panel.h"

namespace ExperimentRedbear {

Panel::Panel(const std::string& id)
    : UIElement(id) {
}

void Panel::addChild(std::shared_ptr<UIElement> child) {
    child->setParent(shared_from_this());
    m_children.push_back(child);
}

void Panel::removeChild(std::shared_ptr<UIElement> child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        (*it)->setParent(nullptr);
        m_children.erase(it);
    }
}

void Panel::update(float deltaTime) {
    for (auto& child : m_children) {
        if (child->isActive()) {
            child->update(deltaTime);
        }
    }
}

void Panel::render() {
    // Panel background rendering would be done here
    // For now, this is a placeholder

    // Render children
    for (auto& child : m_children) {
        if (child->isVisible()) {
            child->render();
        }
    }
}

bool Panel::containsPoint(const glm::vec2& point) const {
    return UIElement::containsPoint(point);
}

} // namespace ExperimentRedbear
