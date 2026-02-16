#include "game/Player.h"
#include "core/Input.h"
#include "core/Config.h"
#include "core/Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace ExperimentRedbear {

Player::Player() : m_flashlight(std::make_unique<Flashlight>()) {
    m_camera.setPosition(m_position);
}

Player::~Player() = default;

void Player::initialize() {
    // Get settings from config
    Config& config = Config::getInstance();
    
    m_camera.setPerspective(
        config.gameplay.fov,
        16.0f / 9.0f,  // Will be updated when window size is known
        0.1f,
        config.graphics.renderDistance
    );
    
    m_position = glm::vec3(0.0f, m_normalHeight, 0.0f);
    m_camera.setPosition(m_position);
    
    m_flashlight->initialize();
    
    LOG_INFO("Player initialized at position: " + 
             std::to_string(m_position.x) + ", " +
             std::to_string(m_position.y) + ", " +
             std::to_string(m_position.z));
}

void Player::update(float deltaTime) {
    // Update state
    if (!m_grounded) {
        m_state = PlayerState::FALLING;
    } else if (m_running) {
        m_state = PlayerState::RUNNING;
    } else if (m_crouching) {
        m_state = PlayerState::CROUCHING;
    } else if (glm::length(glm::vec2(m_velocity.x, m_velocity.z)) > 0.1f) {
        m_state = PlayerState::WALKING;
    } else {
        m_state = PlayerState::IDLE;
    }

    // Update physics
    updatePhysics(deltaTime);

    // Update camera
    updateCamera();

    // Update flashlight
    updateFlashlight();

    // Update head bob
    if (m_state == PlayerState::WALKING || m_state == PlayerState::RUNNING) {
        updateHeadBob(deltaTime);
    }

    // Update stamina
    if (m_running && glm::length(glm::vec2(m_velocity.x, m_velocity.z)) > 0.1f) {
        m_stats.stamina = std::max(0.0f, m_stats.stamina - 15.0f * deltaTime);
        if (m_stats.stamina <= 0.0f) {
            m_running = false;
        }
    } else {
        m_stats.stamina = std::min(m_stats.maxStamina, m_stats.stamina + 10.0f * deltaTime);
    }
}

void Player::fixedUpdate(float fixedDeltaTime) {
    // Physics simulation with fixed timestep
    updatePhysics(fixedDeltaTime);
}

void Player::updatePhysics(float deltaTime) {
    Config& config = Config::getInstance();
    Input& input = Input::getInstance();

    // Get movement direction
    glm::vec3 moveDir(0.0f);

    if (input.isKeyDown(87)) moveDir -= m_camera.getForward(); // W
    if (input.isKeyDown(83)) moveDir += m_camera.getForward(); // S
    if (input.isKeyDown(65)) moveDir -= m_camera.getRight();   // A
    if (input.isKeyDown(68)) moveDir += m_camera.getRight();   // D

    // Normalize movement direction
    if (glm::length(moveDir) > 0.0f) {
        moveDir = glm::normalize(moveDir);
        moveDir.y = 0.0f; // Keep movement on XZ plane
        moveDir = glm::normalize(moveDir);
    }

    // Calculate speed
    float speed = m_walkSpeed;
    if (m_running && m_stats.stamina > 0.0f) {
        speed = m_runSpeed;
    } else if (m_crouching) {
        speed = m_crouchSpeed;
    }

    // Apply movement
    float control = m_grounded ? 1.0f : m_airControl;
    m_velocity.x = glm::mix(m_velocity.x, moveDir.x * speed, control * 10.0f * deltaTime);
    m_velocity.z = glm::mix(m_velocity.z, moveDir.z * speed, control * 10.0f * deltaTime);

    // Apply gravity
    if (!m_grounded) {
        m_velocity.y += m_gravity * deltaTime;
    }

    // Apply friction
    if (m_grounded) {
        m_velocity.x *= m_friction;
        m_velocity.z *= m_friction;
    }

    // Update position
    m_position += m_velocity * deltaTime;

    // Simple ground check
    if (m_position.y <= (m_crouching ? m_crouchHeight : m_normalHeight)) {
        m_position.y = m_crouching ? m_crouchHeight : m_normalHeight;
        m_velocity.y = 0.0f;
        m_grounded = true;
    }
}

void Player::updateCamera() {
    Config& config = Config::getInstance();
    Input& input = Input::getInstance();

    // Mouse look
    glm::vec2 mouseDelta = input.getMouseDelta();
    float sensitivity = config.gameplay.mouseSensitivity;

    if (config.gameplay.invertY) {
        mouseDelta.y = -mouseDelta.y;
    }

    m_yaw += mouseDelta.x * sensitivity;
    m_pitch -= mouseDelta.y * sensitivity;
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    // Update camera rotation
    m_camera.setRotationEuler(m_pitch, m_yaw, 0.0f);

    // Apply head bob offset
    glm::vec3 cameraPos = m_position;
    cameraPos.y += sin(m_headBobTime * m_headBobSpeed) * m_headBobAmount;

    m_camera.setPosition(cameraPos);
}

void Player::updateFlashlight() {
    if (m_flashlightEnabled && m_hasFlashlight) {
        m_flashlight->update(0.0f, m_camera.getPosition(), m_camera.getForward());
    }
}

void Player::updateHeadBob(float deltaTime) {
    float bobSpeed = m_running ? m_headBobSpeed * 1.5f : m_headBobSpeed;
    float bobAmount = m_running ? m_headBobAmount * 1.5f : m_headBobAmount;
    
    m_headBobTime += deltaTime * bobSpeed;
}

void Player::moveForward(float amount) {
    glm::vec3 forward = m_camera.getForward();
    forward.y = 0.0f;
    forward = glm::normalize(forward);
    m_position += forward * amount;
}

void Player::moveRight(float amount) {
    glm::vec3 right = m_camera.getRight();
    right.y = 0.0f;
    right = glm::normalize(right);
    m_position += right * amount;
}

void Player::jump() {
    if (m_grounded && !m_crouching) {
        m_velocity.y = m_jumpForce;
        m_grounded = false;
    }
}

void Player::crouch(bool enable) {
    m_crouching = enable;
}

void Player::run(bool enable) {
    if (enable && m_stats.stamina > 0.0f) {
        m_running = true;
    } else {
        m_running = false;
    }
}

void Player::look(float yaw, float pitch) {
    m_yaw += yaw;
    m_pitch += pitch;
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
}

void Player::toggleFlashlight() {
    if (m_hasFlashlight && m_flashlight->hasBattery()) {
        m_flashlightEnabled = !m_flashlightEnabled;
        m_flashlight->setEnabled(m_flashlightEnabled);
    }
}

void Player::setFlashlightEnabled(bool enabled) {
    m_flashlightEnabled = enabled && m_hasFlashlight;
    m_flashlight->setEnabled(m_flashlightEnabled);
}

void Player::setPosition(const glm::vec3& position) {
    m_position = position;
    m_camera.setPosition(position);
}

} // namespace ExperimentRedbear
