#pragma once

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "game/Interactable.h"

namespace ExperimentRedbear {

enum class ItemType {
    KEY,
    DOCUMENT,
    BATTERY,
    HEALTH_PACK,
    WEAPON,
    AMMO,
    QUEST_ITEM,
    MISC
};

class Item : public Interactable {
public:
    Item(ItemType type, const std::string& name, const std::string& id);
    ~Item();

    bool canInteract() const override;
    void interact() override;
    std::string getInteractionText() const override;

    ItemType getItemType() const { return m_itemType; }
    const std::string& getName() const { return m_name; }
    const std::string& getID() const { return m_id; }
    const std::string& getDescription() const { return m_description; }

    void setDescription(const std::string& desc) { m_description = desc; }

    // Pickup behavior
    bool isPickupable() const { return m_pickupable; }
    void setPickupable(bool pickupable) { m_pickupable = pickupable; }

    // Quantity (for batteries, ammo, etc.)
    int getQuantity() const { return m_quantity; }
    void setQuantity(int qty) { m_quantity = qty; }

    // Visual properties
    void setModelPath(const std::string& path) { m_modelPath = path; }
    const std::string& getModelPath() const { return m_modelPath; }

    // World transform
    const glm::vec3& getPosition() const { return m_position; }
    const glm::vec3& getRotation() const { return m_rotation; }
    const glm::vec3& getScale() const { return m_scale; }

    void setPosition(const glm::vec3& pos);
    void setRotation(const glm::vec3& rot) { m_rotation = rot; }
    void setScale(const glm::vec3& scale) { m_scale = scale; }

    // Floating animation
    void setFloats(bool floats) { m_floats = floats; }
    void update(float deltaTime);

    // Collected state
    bool isCollected() const { return m_collected; }
    void setCollected(bool collected) { m_collected = collected; }

private:
    ItemType m_itemType;
    std::string m_name;
    std::string m_id;
    std::string m_description;
    std::string m_modelPath;

    glm::vec3 m_rotation = glm::vec3(0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);

    bool m_pickupable = true;
    int m_quantity = 1;
    bool m_collected = false;

    // Animation
    bool m_floats = true;
    float m_floatTime = 0.0f;
    float m_floatSpeed = 2.0f;
    float m_floatAmount = 0.1f;
    float m_baseY = 0.0f;
};

// Inventory system
struct InventorySlot {
    std::string itemId;
    int quantity = 0;
};

class Inventory {
public:
    Inventory(int maxSlots = 20);

    bool addItem(std::shared_ptr<Item> item);
    bool removeItem(const std::string& itemId, int quantity = 1);
    bool hasItem(const std::string& itemId) const;
    int getItemCount(const std::string& itemId) const;

    const std::vector<InventorySlot>& getSlots() const { return m_slots; }
    int getMaxSlots() const { return m_maxSlots; }
    int getUsedSlots() const;

private:
    std::vector<InventorySlot> m_slots;
    std::unordered_map<std::string, std::shared_ptr<Item>> m_items;
    int m_maxSlots;
};

} // namespace ExperimentRedbear
