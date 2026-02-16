#include "ui/UIElement.h"

namespace ExperimentRedbear {

UIElement::UIElement(const std::string& id)
    : m_id(id) {
}

bool UIElement::containsPoint(const glm::vec2& point) const {
    return point.x >= m_position.x &&
           point.x <= m_position.x + m_size.x &&
           point.y >= m_position.y &&
           point.y <= m_position.y + m_size.y;
}

} // namespace ExperimentRedbear
