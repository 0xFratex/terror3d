#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace ExperimentRedbear {

struct PhysicsBody {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f);
    glm::vec3 force = glm::vec3(0.0f);

    float mass = 1.0f;
    float inverseMass = 1.0f;
    float damping = 0.98f;

    bool isStatic = false;
    bool useGravity = true;

    void setMass(float m) {
        mass = m;
        inverseMass = (m > 0.0f) ? 1.0f / m : 0.0f;
    }
};

struct Collider {
    enum class Type {
        Box,
        Sphere,
        Capsule,
        Mesh
    };

    Type type = Type::Box;
    glm::vec3 center = glm::vec3(0.0f);
    glm::vec3 size = glm::vec3(1.0f);
    float radius = 0.5f;
    bool isTrigger = false;
};

class PhysicsWorld {
public:
    static PhysicsWorld& getInstance();

    bool initialize();
    void shutdown();

    void update(float deltaTime);

    // Body management
    PhysicsBody* createBody();
    void destroyBody(PhysicsBody* body);

    // Collider management
    Collider* createCollider(PhysicsBody* body, Collider::Type type);
    void destroyCollider(Collider* collider);

    // Physics settings
    void setGravity(const glm::vec3& gravity);
    const glm::vec3& getGravity() const { return m_gravity; }

    // Raycasting
    bool raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance,
                 glm::vec3& hitPoint, glm::vec3& hitNormal);

private:
    PhysicsWorld() = default;
    ~PhysicsWorld() = default;
    PhysicsWorld(const PhysicsWorld&) = delete;
    PhysicsWorld& operator=(const PhysicsWorld&) = delete;

    void integrateForces(PhysicsBody* body, float deltaTime);
    void integrateVelocity(PhysicsBody* body, float deltaTime);

    glm::vec3 m_gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    std::vector<std::unique_ptr<PhysicsBody>> m_bodies;
    std::vector<std::unique_ptr<Collider>> m_colliders;

    bool m_initialized = false;
};

} // namespace ExperimentRedbear
