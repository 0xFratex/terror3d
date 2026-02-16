#include "game/Item.h"
#include <cmath>

namespace ExperimentRedbear {

Item::Item(ItemType type, const std::string& name, const std::string& id)
    : Interactable(InteractableType::ITEM),
      m_itemType(type),
      m_name(name),
      m_id(id) {
    setInteractDistance(2.0f);
}

Item::~Item() = default;

bool Item::canInteract() const {
    return m_active && !m_collected && m_pickupable;
}

void Item::interact() {
    if (!canInteract()) return;

    // This would typically add the item to the player's inventory
    m_collected = true;
    m_active = false;

    if (m_onInteractCallback) {
        m_onInteractCallback();
    }
}

std::string Item::getInteractionText() const {
    if (m_collected) return "";
    return "Press [E] to pick up " + m_name;
}

void Item::setPosition(const glm::vec3& pos) {
    m_position = pos;
    m_baseY = pos.y;
    Interactable::setPosition(pos);
}

void Item::update(float deltaTime) {
    if (!m_active || m_collected) return;

    if (m_floats) {
        m_floatTime += deltaTime * m_floatSpeed;
        m_position.y = m_baseY + std::sin(m_floatTime) * m_floatAmount;
        Interactable::setPosition(m_position);
    }
}

// Inventory implementation

Inventory::Inventory(int maxSlots) : m_maxSlots(maxSlots) {
    m_slots.resize(maxSlots);
}

bool Inventory::addItem(std::shared_ptr<Item> item) {
    if (!item) return false;

    // Check if item already exists (stackable)
    for (auto& slot : m_slots) {
        if (slot.itemId == item->getID()) {
            slot.quantity += item->getQuantity();
            return true;
        }
    }

    // Find empty slot
    for (auto& slot : m_slots) {
        if (slot.itemId.empty()) {
            slot.itemId = item->getID();
            slot.quantity = item->getQuantity();
            m_items[item->getID()] = item;
            return true;
        }
    }

    return false; // Inventory full
}

bool Inventory::removeItem(const std::string& itemId, int quantity) {
    for (auto& slot : m_slots) {
        if (slot.itemId == itemId) {
            slot.quantity -= quantity;
            if (slot.quantity <= 0) {
                slot.itemId = "";
                slot.quantity = 0;
                m_items.erase(itemId);
            }
            return true;
        }
    }
    return false;
}

bool Inventory::hasItem(const std::string& itemId) const {
    for (const auto& slot : m_slots) {
        if (slot.itemId == itemId && slot.quantity > 0) {
            return true;
        }
    }
    return false;
}

int Inventory::getItemCount(const std::string& itemId) const {
    for (const auto& slot : m_slots) {
        if (slot.itemId == itemId) {
            return slot.quantity;
        }
    }
    return 0;
}

int Inventory::getUsedSlots() const {
    int count = 0;
    for (const auto& slot : m_slots) {
        if (!slot.itemId.empty()) {
            count++;
        }
    }
    return count;
}

} // namespace ExperimentRedbear
