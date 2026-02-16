#include "game/Interactable.h"
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ExperimentRedbear {

Interactable::Interactable(InteractableType type) : m_type(type) {}

Interactable::~Interactable() = default;

void Interactable::setPosition(const glm::vec3& pos) {
    m_position = pos;
}

void Interactable::setBounds(const glm::vec3& min, const glm::vec3& max) {
    m_boundsMin = min;
    m_boundsMax = max;
}

bool Interactable::checkInRange(const glm::vec3& playerPos) const {
    float distance = glm::length(m_position - playerPos);
    return distance <= m_interactDistance;
}

bool Interactable::checkLineOfSight(const glm::vec3& playerPos, const glm::vec3& playerDir) const {
    glm::vec3 toInteractable = m_position - playerPos;
    float distance = glm::length(toInteractable);
    if (distance > m_interactDistance) return false;

    toInteractable = glm::normalize(toInteractable);
    float dotProduct = glm::dot(playerDir, toInteractable);
    return dotProduct > 0.7f; // ~45 degree cone
}

} // namespace ExperimentRedbear
