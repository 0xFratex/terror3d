#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace ExperimentRedbear {

class UIElement {
public:
    UIElement(const std::string& id = "");
    virtual ~UIElement() = default;

    // Identification
    const std::string& getId() const { return m_id; }
    void setId(const std::string& id) { m_id = id; }

    // Active state
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

    // Visibility
    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

    // Position and size
    const glm::vec2& getPosition() const { return m_position; }
    void setPosition(const glm::vec2& pos) { m_position = pos; }

    const glm::vec2& getSize() const { return m_size; }
    void setSize(const glm::vec2& size) { m_size = size; }

    // Anchoring
    enum class Anchor {
        TopLeft,
        TopCenter,
        TopRight,
        CenterLeft,
        Center,
        CenterRight,
        BottomLeft,
        BottomCenter,
        BottomRight
    };

    Anchor getAnchor() const { return m_anchor; }
    void setAnchor(Anchor anchor) { m_anchor = anchor; }

    // Virtual methods
    virtual void update(float deltaTime) {}
    virtual void render() {}
    virtual bool containsPoint(const glm::vec2& point) const;

    // Input handling
    virtual void onMouseEnter() {}
    virtual void onMouseLeave() {}
    virtual void onMouseMove(const glm::vec2& pos) {}
    virtual void onMouseClick(int button, bool pressed) {}
    virtual void onKey(int key, bool pressed) {}

    // Parent
    void setParent(std::shared_ptr<UIElement> parent) { m_parent = parent; }
    std::shared_ptr<UIElement> getParent() const { return m_parent.lock(); }

protected:
    std::string m_id;
    bool m_active = true;
    bool m_visible = true;
    glm::vec2 m_position = glm::vec2(0.0f);
    glm::vec2 m_size = glm::vec2(100.0f);
    Anchor m_anchor = Anchor::TopLeft;
    std::weak_ptr<UIElement> m_parent;
};

} // namespace ExperimentRedbear
