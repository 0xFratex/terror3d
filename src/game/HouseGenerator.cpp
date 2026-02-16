#include "game/HouseGenerator.h"
#include "game/World.h"
#include "game/Door.h"
#include "game/Item.h"
#include "core/Logger.h"
#include <random>

namespace ExperimentRedbear {

HouseGenerator::HouseGenerator() {}

HouseGenerator::~HouseGenerator() = default;

void HouseGenerator::generate(World* world, const glm::vec3& position) {
    m_position = position;

    LOG_INFO("Generating house at position: " + 
             std::to_string(position.x) + ", " +
             std::to_string(position.y) + ", " +
             std::to_string(position.z));

    generateLayout();
    generateRooms();
    generateDoors(world);
    generateItems(world);
    generateColliders(world);
}

void HouseGenerator::generateLayout() {
    m_floors.clear();

    // Ground floor
    Floor groundFloor;
    groundFloor.level = 0;
    groundFloor.height = 0.0f;

    // Create rooms for ground floor
    Room bedroom;
    bedroom.name = "Master Bedroom";
    bedroom.position = glm::vec3(0.0f, 0.0f, 0.0f);
    bedroom.size = glm::vec3(5.0f, 3.0f, 5.0f);
    bedroom.doorPositions = {
        glm::vec3(2.5f, 0.0f, 2.5f)  // Door to hallway
    };
    bedroom.itemSpawnPoints = {
        glm::vec3(1.0f, 0.8f, 1.0f),   // Desk (flashlight location)
        glm::vec3(4.0f, 0.5f, 1.0f),   // Bedside table
        glm::vec3(3.5f, 0.0f, 4.0f)    // Closet area
    };
    groundFloor.rooms.push_back(bedroom);

    // Hallway
    Room hallway;
    hallway.name = "Hallway";
    hallway.position = glm::vec3(0.0f, 0.0f, 5.0f);
    hallway.size = glm::vec3(10.0f, 3.0f, 2.0f);
    hallway.doorPositions = {
        glm::vec3(2.5f, 0.0f, 0.0f),   // To bedroom
        glm::vec3(7.5f, 0.0f, 0.0f),   // To bathroom
        glm::vec3(5.0f, 0.0f, 2.0f),   // To stairs
        glm::vec3(0.0f, 0.0f, 1.0f)    // To living room
    };
    groundFloor.rooms.push_back(hallway);

    // Bathroom
    Room bathroom;
    bathroom.name = "Bathroom";
    bathroom.position = glm::vec3(5.0f, 0.0f, 5.0f);
    bathroom.size = glm::vec3(3.0f, 3.0f, 3.0f);
    bathroom.doorPositions = {
        glm::vec3(0.0f, 0.0f, 1.5f)    // Door to hallway
    };
    groundFloor.rooms.push_back(bathroom);

    // Living room
    Room livingRoom;
    livingRoom.name = "Living Room";
    livingRoom.position = glm::vec3(-5.0f, 0.0f, 5.0f);
    livingRoom.size = glm::vec3(5.0f, 3.0f, 6.0f);
    livingRoom.doorPositions = {
        glm::vec3(5.0f, 0.0f, 2.0f),   // To hallway
        glm::vec3(-2.5f, 0.0f, 6.0f)   // To kitchen
    };
    livingRoom.itemSpawnPoints = {
        glm::vec3(-6.0f, 0.4f, 7.0f),  // Coffee table
        glm::vec3(-8.0f, 0.0f, 8.0f)   // Fireplace area
    };
    groundFloor.rooms.push_back(livingRoom);

    // Kitchen
    Room kitchen;
    kitchen.name = "Kitchen";
    kitchen.position = glm::vec3(-5.0f, 0.0f, 10.0f);
    kitchen.size = glm::vec3(5.0f, 3.0f, 4.0f);
    kitchen.doorPositions = {
        glm::vec3(-2.5f, 0.0f, 0.0f),  // To living room
        glm::vec3(-5.0f, 0.0f, 5.0f)   // To outside
    };
    kitchen.itemSpawnPoints = {
        glm::vec3(-7.0f, 0.9f, 11.0f)  // Kitchen counter
    };
    groundFloor.rooms.push_back(kitchen);

    m_floors.push_back(groundFloor);

    // Second floor (attic)
    Floor secondFloor;
    secondFloor.level = 1;
    secondFloor.height = 3.5f;

    Room attic;
    attic.name = "Attic";
    attic.position = glm::vec3(0.0f, 3.5f, 0.0f);
    attic.size = glm::vec3(10.0f, 2.5f, 8.0f);
    attic.doorPositions = {
        glm::vec3(5.0f, 3.5f, 4.0f)    // Stairs down
    };
    attic.itemSpawnPoints = {
        glm::vec3(2.0f, 4.0f, 3.0f),   // Old boxes
        glm::vec3(-3.0f, 4.0f, 2.0f)   // Creepy doll (maybe)
    };
    secondFloor.rooms.push_back(attic);

    m_floors.push_back(secondFloor);
}

void HouseGenerator::generateRooms() {
    // This would generate the actual 3D geometry for each room
    LOG_DEBUG("Generating room geometry...");
}

void HouseGenerator::generateDoors(World* world) {
    LOG_DEBUG("Generating doors...");

    for (const auto& floor : m_floors) {
        for (const auto& room : floor.rooms) {
            for (const auto& doorPos : room.doorPositions) {
                auto door = std::make_shared<Door>(doorPos);
                world->addDoor(door);
            }
        }
    }
}

void HouseGenerator::generateWindows() {
    LOG_DEBUG("Generating windows...");
}

void HouseGenerator::generateFurniture() {
    LOG_DEBUG("Generating furniture...");
}

void HouseGenerator::generateItems(World* world) {
    LOG_DEBUG("Generating items...");

    // Flashlight in bedroom (starting item)
    auto flashlight = std::make_shared<Item>(ItemType::MISC, "Flashlight", "flashlight_01");
    flashlight->setDescription("A battery-powered flashlight. Essential for exploring dark areas.");
    flashlight->setPosition(glm::vec3(1.0f, 0.9f, 1.0f));
    world->addItem(flashlight);

    // Batteries in various locations
    auto battery1 = std::make_shared<Item>(ItemType::BATTERY, "Battery", "battery_01");
    battery1->setDescription("A fresh AA battery. Can be used to recharge the flashlight.");
    battery1->setPosition(glm::vec3(4.0f, 0.6f, 1.0f));
    world->addItem(battery1);

    // Key item
    auto key = std::make_shared<Item>(ItemType::KEY, "Rusty Key", "key_rusty_01");
    key->setDescription("An old rusty key. Might open something important.");
    key->setPosition(glm::vec3(-6.0f, 0.5f, 7.0f));
    world->addItem(key);

    // Document/story item
    auto document = std::make_shared<Item>(ItemType::DOCUMENT, "Research Notes", "doc_research_01");
    document->setDescription("A crumpled piece of paper with illegible handwriting...");
    document->setPosition(glm::vec3(2.0f, 4.1f, 3.0f));
    world->addItem(document);
}

void HouseGenerator::generateColliders(World* world) {
    LOG_DEBUG("Generating house colliders...");

    // Floor collider
    Collider floorCollider;
    floorCollider.min = glm::vec3(-15.0f, -0.1f, -5.0f);
    floorCollider.max = glm::vec3(10.0f, 0.0f, 20.0f);
    floorCollider.tag = "floor";
    world->addCollider(floorCollider);

    // Wall colliders for bedroom
    Collider wall1;
    wall1.min = glm::vec3(-0.1f, 0.0f, 0.0f);
    wall1.max = glm::vec3(0.0f, 3.0f, 5.0f);
    wall1.tag = "wall";
    world->addCollider(wall1);

    Collider wall2;
    wall2.min = glm::vec3(0.0f, 0.0f, -0.1f);
    wall2.max = glm::vec3(5.0f, 3.0f, 0.0f);
    wall2.tag = "wall";
    world->addCollider(wall2);

    // Add more wall colliders as needed...
}

void HouseGenerator::generateLighting() {
    LOG_DEBUG("Generating house lighting...");
}

} // namespace ExperimentRedbear
