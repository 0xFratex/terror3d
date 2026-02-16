#include "game/World.h"
#include "game/HouseGenerator.h"
#include "game/ForestGenerator.h"
#include "core/Logger.h"
#include <algorithm>

namespace ExperimentRedbear {

World::World() {}

World::~World() = default;

bool World::initialize(const WorldSettings& settings) {
    m_settings = settings;
    m_playerStart = settings.playerStart;
    m_playerStartYaw = 0.0f;

    LOG_INFO("Initializing world: " + settings.name);

    return true;
}

void World::update(float deltaTime) {
    // Update all interactables
    for (auto& interactable : m_interactables) {
        if (interactable->isActive()) {
            // Items have their own update
            auto item = std::dynamic_pointer_cast<Item>(interactable);
            if (item) {
                item->update(deltaTime);
            }
        }
    }

    // Update doors
    for (auto& door : m_doors) {
        door->update(deltaTime);
    }
}

void World::render() {
    // Rendering is handled by the renderer
}

void World::setPlayerStart(const glm::vec3& position, float yaw) {
    m_playerStart = position;
    m_playerStartYaw = yaw;
}

void World::addInteractable(std::shared_ptr<Interactable> interactable) {
    m_interactables.push_back(interactable);
}

void World::removeInteractable(const std::string& id) {
    m_interactables.erase(
        std::remove_if(m_interactables.begin(), m_interactables.end(),
            [&id](const std::shared_ptr<Interactable>& inter) {
                // This would need an ID system
                return false;
            }),
        m_interactables.end()
    );
}

std::shared_ptr<Interactable> World::getInteractable(const std::string& id) {
    // Find by ID
    for (auto& inter : m_interactables) {
        // This would need proper ID checking
    }
    return nullptr;
}

std::shared_ptr<Interactable> World::getNearestInteractable(const glm::vec3& playerPos,
                                                             const glm::vec3& playerDir,
                                                             float maxDistance) {
    std::shared_ptr<Interactable> nearest = nullptr;
    float nearestDistance = maxDistance;

    for (auto& interactable : m_interactables) {
        if (!interactable->isActive()) continue;
        if (!interactable->canInteract()) continue;

        float distance = glm::length(interactable->getPosition() - playerPos);
        if (distance < nearestDistance) {
            // Check if in view cone
            if (playerDir != glm::vec3(0.0f)) {
                glm::vec3 toInteractable = glm::normalize(interactable->getPosition() - playerPos);
                float dotProduct = glm::dot(playerDir, toInteractable);
                if (dotProduct < 0.7f) continue; // ~45 degree cone
            }

            nearestDistance = distance;
            nearest = interactable;
        }
    }

    return nearest;
}

void World::addDoor(std::shared_ptr<Door> door) {
    m_doors.push_back(door);
    m_interactables.push_back(door);
}

std::shared_ptr<Door> World::getDoor(const std::string& id) {
    for (auto& door : m_doors) {
        // This would need proper ID checking
    }
    return nullptr;
}

void World::addItem(std::shared_ptr<Item> item) {
    m_items.push_back(item);
    m_interactables.push_back(item);
}

std::shared_ptr<Item> World::getItem(const std::string& id) {
    for (auto& item : m_items) {
        if (item->getID() == id) {
            return item;
        }
    }
    return nullptr;
}

void World::removeItem(const std::string& id) {
    m_items.erase(
        std::remove_if(m_items.begin(), m_items.end(),
            [&id](const std::shared_ptr<Item>& item) {
                return item->getID() == id;
            }),
        m_items.end()
    );
}

void World::addCollider(const Collider& collider) {
    m_colliders.push_back(collider);
}

bool World::checkCollision(const glm::vec3& position, float radius,
                           glm::vec3& outNormal, glm::vec3& outPoint) const {
    for (const auto& collider : m_colliders) {
        // Simple AABB vs sphere collision
        glm::vec3 closest = glm::clamp(position, collider.min, collider.max);
        float distance = glm::length(closest - position);

        if (distance < radius) {
            if (distance > 0.0f) {
                outNormal = glm::normalize(position - closest);
            } else {
                outNormal = glm::vec3(0.0f, 1.0f, 0.0f);
            }
            outPoint = closest;
            return true;
        }
    }
    return false;
}

bool World::checkRaycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance,
                         glm::vec3& outHit, std::string& outTag) const {
    float nearestDistance = maxDistance;
    bool hit = false;

    for (const auto& collider : m_colliders) {
        // Ray vs AABB intersection
        glm::vec3 invDir = 1.0f / direction;
        glm::vec3 t1 = (collider.min - origin) * invDir;
        glm::vec3 t2 = (collider.max - origin) * invDir;

        glm::vec3 tMin = glm::min(t1, t2);
        glm::vec3 tMax = glm::max(t1, t2);

        float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
        float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

        if (tNear <= tFar && tNear < nearestDistance && tNear > 0.0f) {
            nearestDistance = tNear;
            outHit = origin + direction * tNear;
            outTag = collider.tag;
            hit = true;
        }
    }

    return hit;
}

void World::generateColliders() {
    // Generate colliders for walls, floors, and other static geometry
    LOG_DEBUG("Generating world colliders...");
}

} // namespace ExperimentRedbear
