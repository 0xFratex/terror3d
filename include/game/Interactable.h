#pragma once

#include <glm/glm.hpp>
#include <string>
#include <functional>

namespace ExperimentRedbear {

enum class InteractableType {
    DOOR,
    ITEM,
    SWITCH,
    CONTAINER,
    READABLE,
    GENERIC
};

class Interactable {
public:
    Interactable(InteractableType type);
    virtual ~Interactable();

    virtual bool canInteract() const = 0;
    virtual void interact() = 0;
    virtual std::string getInteractionText() const = 0;

    InteractableType getType() const { return m_type; }

    const glm::vec3& getPosition() const { return m_position; }
    const glm::vec3& getBoundsMin() const { return m_boundsMin; }
    const glm::vec3& getBoundsMax() const { return m_boundsMax; }

    void setPosition(const glm::vec3& pos);
    void setBounds(const glm::vec3& min, const glm::vec3& max);

    float getInteractDistance() const { return m_interactDistance; }
    void setInteractDistance(float distance) { m_interactDistance = distance; }

    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

    bool isHighlighted() const { return m_highlighted; }
    void setHighlighted(bool highlighted) { m_highlighted = highlighted; }

    void setOnInteractCallback(std::function<void()> callback) { m_onInteractCallback = callback; }

    bool checkInRange(const glm::vec3& playerPos) const;
    bool checkLineOfSight(const glm::vec3& playerPos, const glm::vec3& playerDir) const;

protected:
    InteractableType m_type;
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_boundsMin = glm::vec3(-0.5f);
    glm::vec3 m_boundsMax = glm::vec3(0.5f);
    float m_interactDistance = 2.5f;
    bool m_active = true;
    bool m_highlighted = false;
    std::function<void()> m_onInteractCallback;
};

} // namespace ExperimentRedbear
