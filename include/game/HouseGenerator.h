#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "game/World.h"

namespace ExperimentRedbear {

struct Room {
    std::string name;
    glm::vec3 position;
    glm::vec3 size;
    std::vector<glm::vec3> doorPositions;
    std::vector<glm::vec3> windowPositions;
    std::vector<glm::vec3> itemSpawnPoints;
};

struct Floor {
    int level;
    float height;
    std::vector<Room> rooms;
};

class HouseGenerator {
public:
    HouseGenerator();
    ~HouseGenerator();

    void generate(World* world, const glm::vec3& position);

    const std::vector<Floor>& getFloors() const { return m_floors; }
    const glm::vec3& getHouseSize() const { return m_houseSize; }

    // Configuration
    void setNumFloors(int floors) { m_numFloors = floors; }
    void setHouseSize(const glm::vec3& size) { m_houseSize = size; }
    void setStyle(int style) { m_style = style; }

private:
    void generateLayout();
    void generateRooms();
    void generateDoors(World* world);
    void generateWindows();
    void generateFurniture();
    void generateItems(World* world);
    void generateColliders(World* world);
    void generateLighting();

    glm::vec3 m_position = glm::vec3(0.0f);
    glm::vec3 m_houseSize = glm::vec3(15.0f, 6.0f, 12.0f);
    int m_numFloors = 2;
    int m_style = 0; // 0=abandoned, 1=victorian, 2=modern

    std::vector<Floor> m_floors;

    // Room templates
    Room m_bedroom;
    Room m_bathroom;
    Room m_kitchen;
    Room m_livingRoom;
    Room m_hallway;
    Room m_attic;
    Room m_basement;
};

} // namespace ExperimentRedbear
