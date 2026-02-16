#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "game/Interactable.h"

namespace ExperimentRedbear {

enum class DoorState {
    CLOSED,
    OPENING,
    OPEN,
    CLOSING,
    LOCKED
};

class Door : public Interactable {
public:
    Door(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f));
    ~Door();

    void update(float deltaTime);

    // Interaction
    bool canInteract() const override;
    void interact() override;
    std::string getInteractionText() const override;

    // State
    DoorState getState() const { return m_state; }
    bool isOpen() const { return m_state == DoorState::OPEN || m_state == DoorState::OPENING; }
    bool isLocked() const { return m_state == DoorState::LOCKED; }

    // Locking
    void lock();
    void unlock();
    void setLocked(bool locked);

    // Configuration
    void setOpenAngle(float angle) { m_openAngle = angle; }
    void setOpenSpeed(float speed) { m_openSpeed = speed; }
    void setRequiresKey(const std::string& keyId) { m_requiredKey = keyId; }

    // Transform
    const glm::vec3& getPosition() const { return m_position; }
    const glm::vec3& getRotation() const { return m_rotation; }
    void setPosition(const glm::vec3& pos);
    void setRotation(const glm::vec3& rot);

    // Collision
    bool checkCollision(const glm::vec3& point, float radius) const;

private:
    void updateAnimation(float deltaTime);

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale = glm::vec3(1.0f);

    DoorState m_state = DoorState::CLOSED;
    float m_currentAngle = 0.0f;
    float m_openAngle = 90.0f;
    float m_openSpeed = 120.0f; // degrees per second
    bool m_openOutward = true;

    std::string m_requiredKey;

    // Collision box
    glm::vec3 m_collisionMin;
    glm::vec3 m_collisionMax;
    float m_width = 1.0f;
    float m_height = 2.2f;
    float m_depth = 0.1f;
};

} // namespace ExperimentRedbear
