#include "game/ForestGenerator.h"
#include "game/World.h"
#include "core/Logger.h"
#include <random>
#include <cmath>

namespace ExperimentRedbear {

ForestGenerator::ForestGenerator() : m_noise(42) {}

ForestGenerator::~ForestGenerator() = default;

void ForestGenerator::generate(World* world, const glm::vec3& center, float radius) {
    m_center = center;
    m_radius = radius;

    LOG_INFO("Generating forest around: " + 
             std::to_string(center.x) + ", " +
             std::to_string(center.y) + ", " +
             std::to_string(center.z) +
             " with radius: " + std::to_string(radius));

    generateTerrain(world);
    generateTrees();
    generateSnow();
}

void ForestGenerator::generateTerrain(World* world) {
    LOG_DEBUG("Generating terrain...");

    // Generate ground collider
    Collider groundCollider;
    groundCollider.min = glm::vec3(
        m_center.x - m_radius,
        -0.5f,
        m_center.z - m_radius
    );
    groundCollider.max = glm::vec3(
        m_center.x + m_radius,
        0.0f,
        m_center.z + m_radius
    );
    groundCollider.tag = "ground";
    world->addCollider(groundCollider);

    // Generate ground mesh vertices
    int gridSize = 100;
    float cellSize = (m_radius * 2.0f) / gridSize;

    for (int i = 0; i <= gridSize; i++) {
        for (int j = 0; j <= gridSize; j++) {
            float x = m_center.x - m_radius + i * cellSize;
            float z = m_center.z - m_radius + j * cellSize;

            // Use Perlin noise for terrain height variation
            float height = m_noise.octaveNoise(x * 0.01f, z * 0.01f, 4, 0.5f) * 0.5f;

            m_groundVertices.push_back(glm::vec3(x, height, z));
        }
    }

    // Generate indices
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            int idx = i * (gridSize + 1) + j;
            m_groundIndices.push_back(idx);
            m_groundIndices.push_back(idx + gridSize + 1);
            m_groundIndices.push_back(idx + 1);

            m_groundIndices.push_back(idx + 1);
            m_groundIndices.push_back(idx + gridSize + 1);
            m_groundIndices.push_back(idx + gridSize + 2);
        }
    }
}

void ForestGenerator::generateTrees() {
    LOG_DEBUG("Generating trees...");

    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-m_radius, m_radius);
    std::uniform_real_distribution<float> heightDist(5.0f, 15.0f);
    std::uniform_real_distribution<float> radiusDist(0.1f, 0.3f);
    std::uniform_int_distribution<int> typeDist(0, 2);
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);

    // Calculate number of trees based on density
    float area = 3.14159f * m_radius * m_radius;
    int numTrees = static_cast<int>(area * m_treeDensity);

    // Clear area around the house
    float houseClearRadius = 15.0f;

    m_trees.reserve(numTrees);
    int attempts = 0;
    int maxAttempts = numTrees * 10;

    while (static_cast<int>(m_trees.size()) < numTrees && attempts < maxAttempts) {
        attempts++;

        glm::vec3 pos(
            m_center.x + dist(rng),
            0.0f,
            m_center.z + dist(rng)
        );

        // Check if too close to house
        float distFromCenter = glm::length(glm::vec2(pos.x - m_center.x, pos.z - m_center.z));
        if (distFromCenter < houseClearRadius) continue;

        // Check if too close to other trees
        bool tooClose = false;
        for (const auto& tree : m_trees) {
            float dist = glm::length(glm::vec2(tree.position.x - pos.x, tree.position.z - pos.z));
            if (dist < 2.0f) {
                tooClose = true;
                break;
            }
        }
        if (tooClose) continue;

        Tree tree;
        tree.position = pos;
        tree.height = heightDist(rng);
        tree.trunkRadius = radiusDist(rng);
        tree.canopyRadius = tree.height * 0.3f;
        tree.type = typeDist(rng);
        tree.rotation = rotDist(rng);

        m_trees.push_back(tree);
    }

    LOG_INFO("Generated " + std::to_string(m_trees.size()) + " trees");
}

void ForestGenerator::generatePaths() {
    LOG_DEBUG("Generating forest paths...");
}

void ForestGenerator::generateSnow() {
    LOG_DEBUG("Generating snow particles...");

    m_snowflakes.clear();
    m_snowflakes.reserve(m_maxSnowflakes);

    std::mt19937 rng(123);
    std::uniform_real_distribution<float> posDist(-m_snowAreaRadius, m_snowAreaRadius);
    std::uniform_real_distribution<float> sizeDist(0.02f, 0.08f);
    std::uniform_real_distribution<float> speedDist(0.5f, 2.0f);

    for (int i = 0; i < m_maxSnowflakes; i++) {
        Snowflake flake;
        flake.position = glm::vec3(
            posDist(rng),
            static_cast<float>(rng() % 100) / 100.0f * 50.0f,  // Y: 0 to 50
            posDist(rng)
        );
        flake.velocity = glm::vec3(
            static_cast<float>(rng() % 100) / 100.0f - 0.5f,  // Slight horizontal drift
            -speedDist(rng),
            static_cast<float>(rng() % 100) / 100.0f - 0.5f
        );
        flake.size = sizeDist(rng);
        flake.opacity = 0.5f + static_cast<float>(rng() % 100) / 200.0f;
        flake.rotation = static_cast<float>(rng() % 360);

        m_snowflakes.push_back(flake);
    }

    LOG_INFO("Generated " + std::to_string(m_snowflakes.size()) + " snow particles");
}

void ForestGenerator::update(float deltaTime, const glm::vec3& playerPos) {
    updateSnow(deltaTime, playerPos);
}

void ForestGenerator::updateSnow(float deltaTime, const glm::vec3& playerPos) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> posDist(-m_snowAreaRadius, m_snowAreaRadius);

    for (auto& flake : m_snowflakes) {
        // Update position
        flake.position += flake.velocity * deltaTime;
        flake.rotation += 30.0f * deltaTime;  // Slow rotation

        // Reset if below ground or too far from player
        if (flake.position.y < 0.0f) {
            flake.position.y = 50.0f;
            flake.position.x = playerPos.x + posDist(rng);
            flake.position.z = playerPos.z + posDist(rng);
        }

        // Keep snow around player
        float distX = flake.position.x - playerPos.x;
        float distZ = flake.position.z - playerPos.z;
        if (std::abs(distX) > m_snowAreaRadius || std::abs(distZ) > m_snowAreaRadius) {
            flake.position.x = playerPos.x + posDist(rng);
            flake.position.z = playerPos.z + posDist(rng);
            flake.position.y = static_cast<float>(rng() % 100) / 100.0f * 50.0f;
        }
    }
}

} // namespace ExperimentRedbear
