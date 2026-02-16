#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include "graphics/Camera.h"
#include "game/Flashlight.h"

namespace ExperimentRedbear {

enum class PlayerState {
    IDLE,
    WALKING,
    RUNNING,
    CROUCHING,
    JUMPING,
    FALLING
};

struct PlayerStats {
    float health = 100.0f;
    float maxHealth = 100.0f;
    float stamina = 100.0f;
    float maxStamina = 100.0f;
    float sanity = 100.0f;
    float maxSanity = 100.0f;
};

class Player {
public:
    Player();
    ~Player();

    void initialize();
    void update(float deltaTime);
    void fixedUpdate(float fixedDeltaTime);

    // Movement
    void moveForward(float amount);
    void moveRight(float amount);
    void jump();
    void crouch(bool enable);
    void run(bool enable);

    // Look
    void look(float yaw, float pitch);

    // Flashlight
    void toggleFlashlight();
    void setFlashlightEnabled(bool enabled);
    bool hasFlashlight() const { return m_hasFlashlight; }
    bool isFlashlightOn() const { return m_flashlightEnabled; }

    // Camera
    Camera& getCamera() { return m_camera; }
    const Camera& getCamera() const { return m_camera; }

    // Position & Rotation
    const glm::vec3& getPosition() const { return m_position; }
    void setPosition(const glm::vec3& position);
    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }

    // Physics
    const glm::vec3& getVelocity() const { return m_velocity; }
    bool isGrounded() const { return m_grounded; }

    // Stats
    PlayerStats& getStats() { return m_stats; }
    const PlayerStats& getStats() const { return m_stats; }

    // State
    PlayerState getState() const { return m_state; }

    // Collision
    float getHeight() const { return m_height; }
    float getRadius() const { return m_radius; }

    // Head bob
    float getHeadBobTime() const { return m_headBobTime; }

private:
    void updatePhysics(float deltaTime);
    void updateCamera();
    void updateFlashlight();
    void updateHeadBob(float deltaTime);

    // Transform
    glm::vec3 m_position = glm::vec3(0.0f, 1.7f, 0.0f);
    glm::vec3 m_velocity = glm::vec3(0.0f);
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;

    // Physics
    bool m_grounded = true;
    float m_height = 1.7f;
    float m_radius = 0.3f;
    float m_mass = 70.0f;

    // Movement params
    float m_walkSpeed = 3.5f;
    float m_runSpeed = 6.0f;
    float m_crouchSpeed = 1.5f;
    float m_jumpForce = 5.0f;
    float m_gravity = -20.0f;
    float m_friction = 0.9f;
    float m_airControl = 0.3f;

    // Camera
    Camera m_camera;
    float m_normalHeight = 1.7f;
    float m_crouchHeight = 1.0f;

    // Flashlight
    bool m_hasFlashlight = true;
    bool m_flashlightEnabled = false;
    std::unique_ptr<Flashlight> m_flashlight;

    // State
    PlayerState m_state = PlayerState::IDLE;
    PlayerStats m_stats;

    // Animation
    float m_headBobTime = 0.0f;
    float m_headBobAmount = 0.05f;
    float m_headBobSpeed = 10.0f;
    bool m_crouching = false;
    bool m_running = false;
};

} // namespace ExperimentRedbear
