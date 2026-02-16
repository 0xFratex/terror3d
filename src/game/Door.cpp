#include "game/Door.h"
#include "core/Logger.h"
#include <cmath>

namespace ExperimentRedbear {

Door::Door(const glm::vec3& position, const glm::vec3& rotation)
    : Interactable(InteractableType::DOOR),
      m_position(position),
      m_rotation(rotation) {
    setInteractDistance(2.5f);
    updateAnimation(0.0f);
}

Door::~Door() = default;

void Door::update(float deltaTime) {
    updateAnimation(deltaTime);
}

void Door::updateAnimation(float deltaTime) {
    float targetAngle = 0.0f;

    switch (m_state) {
        case DoorState::OPENING:
            targetAngle = m_openOutward ? m_openAngle : -m_openAngle;
            if (std::abs(m_currentAngle - targetAngle) < 1.0f) {
                m_currentAngle = targetAngle;
                m_state = DoorState::OPEN;
            } else {
                m_currentAngle += (targetAngle > m_currentAngle ? 1.0f : -1.0f) * m_openSpeed * deltaTime;
            }
            break;

        case DoorState::CLOSING:
            targetAngle = 0.0f;
            if (std::abs(m_currentAngle) < 1.0f) {
                m_currentAngle = 0.0f;
                m_state = DoorState::CLOSED;
            } else {
                m_currentAngle += (targetAngle > m_currentAngle ? 1.0f : -1.0f) * m_openSpeed * deltaTime;
            }
            break;

        default:
            break;
    }

    // Update collision bounds
    float radians = glm::radians(m_currentAngle);
    float halfWidth = m_width * 0.5f;

    // Calculate door corners based on rotation
    glm::vec3 pivot = m_position;
    glm::vec3 edge = glm::vec3(
        std::sin(radians) * halfWidth,
        0.0f,
        std::cos(radians) * halfWidth
    );

    m_collisionMin = glm::vec3(
        std::min(pivot.x - edge.x, pivot.x + edge.x) - m_depth * 0.5f,
        m_position.y - m_height * 0.5f,
        std::min(pivot.z - edge.z, pivot.z + edge.z) - m_depth * 0.5f
    );
    m_collisionMax = glm::vec3(
        std::max(pivot.x - edge.x, pivot.x + edge.x) + m_depth * 0.5f,
        m_position.y + m_height * 0.5f,
        std::max(pivot.z - edge.z, pivot.z + edge.z) + m_depth * 0.5f
    );

    Interactable::setPosition(m_position);
    Interactable::setBounds(m_collisionMin, m_collisionMax);
}

bool Door::canInteract() const {
    return m_state != DoorState::LOCKED && m_state != DoorState::OPENING && m_state != DoorState::CLOSING;
}

void Door::interact() {
    if (!canInteract()) return;

    switch (m_state) {
        case DoorState::CLOSED:
            m_state = DoorState::OPENING;
            break;
        case DoorState::OPEN:
            m_state = DoorState::CLOSING;
            break;
        default:
            break;
    }

    if (m_onInteractCallback) {
        m_onInteractCallback();
    }
}

std::string Door::getInteractionText() const {
    switch (m_state) {
        case DoorState::CLOSED:
            return "Press [E] to open door";
        case DoorState::OPEN:
            return "Press [E] to close door";
        case DoorState::LOCKED:
            return "Door is locked";
        case DoorState::OPENING:
        case DoorState::CLOSING:
            return "";
        default:
            return "Press [E] to interact";
    }
}

void Door::lock() {
    m_state = DoorState::LOCKED;
}

void Door::unlock() {
    if (m_state == DoorState::LOCKED) {
        m_state = DoorState::CLOSED;
    }
}

void Door::setLocked(bool locked) {
    if (locked) {
        lock();
    } else {
        unlock();
    }
}

void Door::setPosition(const glm::vec3& pos) {
    m_position = pos;
}

void Door::setRotation(const glm::vec3& rot) {
    m_rotation = rot;
}

bool Door::checkCollision(const glm::vec3& point, float radius) const {
    return Interactable::checkInRange(point);
}

} // namespace ExperimentRedbear
