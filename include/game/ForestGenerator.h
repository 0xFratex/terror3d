#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "game/World.h"
#include "utils/PerlinNoise.h"

namespace ExperimentRedbear {

struct Tree {
    glm::vec3 position;
    float height;
    float trunkRadius;
    float canopyRadius;
    int type; // 0=pine, 1=oak, 2=dead
    float rotation;
};

struct Snowflake {
    glm::vec3 position;
    glm::vec3 velocity;
    float size;
    float opacity;
    float rotation;
};

class ForestGenerator {
public:
    ForestGenerator();
    ~ForestGenerator();

    void generate(World* world, const glm::vec3& center, float radius);
    void update(float deltaTime, const glm::vec3& playerPos);

    const std::vector<Tree>& getTrees() const { return m_trees; }
    const std::vector<Snowflake>& getSnowflakes() const { return m_snowflakes; }

    // Configuration
    void setTreeDensity(float density) { m_treeDensity = density; }
    void setSnowIntensity(float intensity) { m_snowIntensity = intensity; }
    void setFogDensity(float density) { m_fogDensity = density; }

private:
    void generateTerrain(World* world);
    void generateTrees();
    void generatePaths();
    void generateSnow();
    void updateSnow(float deltaTime, const glm::vec3& playerPos);

    glm::vec3 m_center = glm::vec3(0.0f);
    float m_radius = 200.0f;
    float m_treeDensity = 0.02f;

    std::vector<Tree> m_trees;
    std::vector<Snowflake> m_snowflakes;

    PerlinNoise m_noise;

    float m_snowIntensity = 1.0f;
    float m_fogDensity = 0.015f;
    int m_maxSnowflakes = 5000;
    float m_snowAreaRadius = 50.0f;

    // Ground
    std::vector<glm::vec3> m_groundVertices;
    std::vector<unsigned int> m_groundIndices;
};

} // namespace ExperimentRedbear
