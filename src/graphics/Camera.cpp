#include "graphics/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cmath>

namespace ExperimentRedbear {

Camera::Camera() {
    updateVectors();
    setPerspective(75.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
}

void Camera::setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
    m_mode = CameraMode::PERSPECTIVE;
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    m_mode = CameraMode::ORTHOGRAPHIC;
    m_left = left;
    m_right = right;
    m_bottom = bottom;
    m_top = top;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_projectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
}

void Camera::setPosition(const glm::vec3& position) {
    m_position = position;
}

void Camera::setRotation(const glm::quat& rotation) {
    m_rotation = rotation;
    updateVectors();
}

void Camera::setRotationEuler(float pitch, float yaw, float roll) {
    m_rotation = glm::quat(glm::vec3(glm::radians(pitch), glm::radians(yaw), glm::radians(roll)));
    updateVectors();
}

void Camera::lookAt(const glm::vec3& target) {
    glm::mat4 lookMatrix = glm::lookAt(m_position, target, glm::vec3(0.0f, 1.0f, 0.0f));
    m_rotation = glm::toQuat(lookMatrix);
    updateVectors();
}

void Camera::move(const glm::vec3& offset) {
    m_position += offset;
}

void Camera::rotate(float pitch, float yaw) {
    // Apply rotation around local axes
    glm::quat yawRotation = glm::angleAxis(glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchRotation = glm::angleAxis(glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    
    m_rotation = yawRotation * m_rotation * pitchRotation;
    m_rotation = glm::normalize(m_rotation);
    updateVectors();
}

void Camera::zoom(float amount) {
    if (m_mode == CameraMode::PERSPECTIVE) {
        m_fov = glm::clamp(m_fov - amount, 10.0f, 120.0f);
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    }
}

void Camera::update() {
    // Update view matrix
    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
    m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    updateFrustum();
}

void Camera::updateVectors() {
    // Calculate forward, right, and up vectors from quaternion
    m_forward = m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    m_right = m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    m_up = m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::updateFrustum() {
    // Extract frustum planes from view-projection matrix
    glm::mat4 m = m_viewProjectionMatrix;

    // Left plane
    m_frustumPlanes[0] = glm::vec4(
        m[0][3] + m[0][0],
        m[1][3] + m[1][0],
        m[2][3] + m[2][0],
        m[3][3] + m[3][0]
    );

    // Right plane
    m_frustumPlanes[1] = glm::vec4(
        m[0][3] - m[0][0],
        m[1][3] - m[1][0],
        m[2][3] - m[2][0],
        m[3][3] - m[3][0]
    );

    // Bottom plane
    m_frustumPlanes[2] = glm::vec4(
        m[0][3] + m[0][1],
        m[1][3] + m[1][1],
        m[2][3] + m[2][1],
        m[3][3] + m[3][1]
    );

    // Top plane
    m_frustumPlanes[3] = glm::vec4(
        m[0][3] - m[0][1],
        m[1][3] - m[1][1],
        m[2][3] - m[2][1],
        m[3][3] - m[3][1]
    );

    // Near plane
    m_frustumPlanes[4] = glm::vec4(
        m[0][3] + m[0][2],
        m[1][3] + m[1][2],
        m[2][3] + m[2][2],
        m[3][3] + m[3][2]
    );

    // Far plane
    m_frustumPlanes[5] = glm::vec4(
        m[0][3] - m[0][2],
        m[1][3] - m[1][2],
        m[2][3] - m[2][2],
        m[3][3] - m[3][2]
    );

    // Normalize planes
    for (int i = 0; i < 6; i++) {
        float length = glm::length(glm::vec3(m_frustumPlanes[i]));
        m_frustumPlanes[i] /= length;
    }
}

bool Camera::isInFrustum(const glm::vec3& point) const {
    for (int i = 0; i < 6; i++) {
        float distance = glm::dot(glm::vec3(m_frustumPlanes[i]), point) + m_frustumPlanes[i].w;
        if (distance < 0.0f) return false;
    }
    return true;
}

bool Camera::isInFrustum(const glm::vec3& center, float radius) const {
    for (int i = 0; i < 6; i++) {
        float distance = glm::dot(glm::vec3(m_frustumPlanes[i]), center) + m_frustumPlanes[i].w;
        if (distance < -radius) return false;
    }
    return true;
}

void Camera::setFOV(float fov) {
    m_fov = fov;
    if (m_mode == CameraMode::PERSPECTIVE) {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    }
}

void Camera::setAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
    if (m_mode == CameraMode::PERSPECTIVE) {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    }
}

void Camera::setNearFarPlanes(float nearPlane, float farPlane) {
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    if (m_mode == CameraMode::PERSPECTIVE) {
        m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
    }
}

} // namespace ExperimentRedbear
