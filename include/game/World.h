#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include "game/Door.h"
#include "game/Item.h"
#include "game/Interactable.h"

namespace ExperimentRedbear {

struct WorldSettings {
    std::string name = "Experiment Redbear";
    glm::vec3 playerStart = glm::vec3(0.0f, 1.7f, 0.0f);
    float playerStartYaw = 0.0f;
    bool enableSnow = true;
    float snowIntensity = 1.0f;
    bool enableFog = true;
    float fogDensity = 0.02f;
    glm::vec3 fogColor = glm::vec3(0.1f, 0.1f, 0.12f);
    float ambientLight = 0.02f;
};

struct Collider {
    glm::vec3 min;
    glm::vec3 max;
    std::string tag;
};

class World {
public:
    World();
    ~World();

    bool initialize(const WorldSettings& settings);
    void update(float deltaTime);
    void render();

    // Player
    void setPlayerStart(const glm::vec3& position, float yaw = 0.0f);
    const glm::vec3& getPlayerStart() const { return m_playerStart; }
    float getPlayerStartYaw() const { return m_playerStartYaw; }

    // Interactables
    void addInteractable(std::shared_ptr<Interactable> interactable);
    void removeInteractable(const std::string& id);
    std::shared_ptr<Interactable> getInteractable(const std::string& id);
    const std::vector<std::shared_ptr<Interactable>>& getInteractables() const { return m_interactables; }

    std::shared_ptr<Interactable> getNearestInteractable(const glm::vec3& playerPos, 
                                                          const glm::vec3& playerDir,
                                                          float maxDistance = 2.5f);

    // Doors
    void addDoor(std::shared_ptr<Door> door);
    std::shared_ptr<Door> getDoor(const std::string& id);

    // Items
    void addItem(std::shared_ptr<Item> item);
    std::shared_ptr<Item> getItem(const std::string& id);
    void removeItem(const std::string& id);

    // Collision
    void addCollider(const Collider& collider);
    bool checkCollision(const glm::vec3& position, float radius, glm::vec3& outNormal, glm::vec3& outPoint) const;
    bool checkRaycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance,
                      glm::vec3& outHit, std::string& outTag) const;

    // Settings
    const WorldSettings& getSettings() const { return m_settings; }

    // Atmosphere
    void setSnowIntensity(float intensity) { m_settings.snowIntensity = intensity; }
    void setFogDensity(float density) { m_settings.fogDensity = density; }

    // Time of day (for future expansion)
    float getTimeOfDay() const { return m_timeOfDay; }
    void setTimeOfDay(float time) { m_timeOfDay = time; }

private:
    void generateColliders();

    WorldSettings m_settings;
    glm::vec3 m_playerStart = glm::vec3(0.0f, 1.7f, 0.0f);
    float m_playerStartYaw = 0.0f;

    std::vector<std::shared_ptr<Interactable>> m_interactables;
    std::vector<std::shared_ptr<Door>> m_doors;
    std::vector<std::shared_ptr<Item>> m_items;
    std::vector<Collider> m_colliders;

    float m_timeOfDay = 22.0f; // 10 PM - night time
};

} // namespace ExperimentRedbear
