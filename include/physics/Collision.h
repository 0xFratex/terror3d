#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace ExperimentRedbear {

struct CollisionInfo {
    bool hit = false;
    glm::vec3 point = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    float distance = 0.0f;
    void* colliderA = nullptr;
    void* colliderB = nullptr;
};

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
    float maxDistance = 1000.0f;
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB() = default;
    AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint)
        : min(minPoint), max(maxPoint) {}

    glm::vec3 getCenter() const { return (min + max) * 0.5f; }
    glm::vec3 getSize() const { return max - min; }
    glm::vec3 getExtents() const { return getSize() * 0.5f; }

    bool contains(const glm::vec3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }

    bool intersects(const AABB& other) const {
        return min.x <= other.max.x && max.x >= other.min.x &&
               min.y <= other.max.y && max.y >= other.min.y &&
               min.z <= other.max.z && max.z >= other.min.z;
    }
};

struct Sphere {
    glm::vec3 center;
    float radius;

    Sphere() = default;
    Sphere(const glm::vec3& c, float r) : center(c), radius(r) {}

    bool contains(const glm::vec3& point) const {
        return glm::length(point - center) <= radius;
    }

    bool intersects(const Sphere& other) const {
        float distance = glm::length(center - other.center);
        return distance <= radius + other.radius;
    }

    bool intersects(const AABB& box) const {
        glm::vec3 closest = glm::clamp(center, box.min, box.max);
        float distance = glm::length(center - closest);
        return distance < radius;
    }
};

// Collision detection functions
namespace Collision {

// AABB vs AABB
bool testAABBvsAABB(const AABB& a, const AABB& b, CollisionInfo& info);

// Sphere vs Sphere
bool testSphereVsSphere(const Sphere& a, const Sphere& b, CollisionInfo& info);

// Sphere vs AABB
bool testSphereVsAABB(const Sphere& sphere, const AABB& box, CollisionInfo& info);

// Ray vs AABB
bool testRayVsAABB(const Ray& ray, const AABB& box, CollisionInfo& info);

// Ray vs Sphere
bool testRayVsSphere(const Ray& ray, const Sphere& sphere, CollisionInfo& info);

// Point vs AABB
bool testPointVsAABB(const glm::vec3& point, const AABB& box);

// Point vs Sphere
bool testPointVsSphere(const glm::vec3& point, const Sphere& sphere);

// Get closest point on AABB to a point
glm::vec3 closestPointOnAABB(const glm::vec3& point, const AABB& box);

// Get closest point on sphere to a point
glm::vec3 closestPointOnSphere(const glm::vec3& point, const Sphere& sphere);

} // namespace Collision

} // namespace ExperimentRedbear
