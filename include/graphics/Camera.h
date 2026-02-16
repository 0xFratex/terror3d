#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ExperimentRedbear {

enum class CameraMode {
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Camera {
public:
    Camera();

    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::quat& rotation);
    void setRotationEuler(float pitch, float yaw, float roll);
    void lookAt(const glm::vec3& target);

    void move(const glm::vec3& offset);
    void rotate(float pitch, float yaw);
    void zoom(float amount);

    void update();

    glm::mat4 getViewMatrix() const { return m_viewMatrix; }
    glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
    glm::mat4 getViewProjectionMatrix() const { return m_viewProjectionMatrix; }

    glm::vec3 getPosition() const { return m_position; }
    glm::quat getRotation() const { return m_rotation; }
    glm::vec3 getForward() const { return m_forward; }
    glm::vec3 getRight() const { return m_right; }
    glm::vec3 getUp() const { return m_up; }

    float getFOV() const { return m_fov; }
    float getAspectRatio() const { return m_aspectRatio; }
    float getNearPlane() const { return m_nearPlane; }
    float getFarPlane() const { return m_farPlane; }

    void setFOV(float fov);
    void setAspectRatio(float aspectRatio);
    void setNearFarPlanes(float nearPlane, float farPlane);

    // Frustum culling
    bool isInFrustum(const glm::vec3& point) const;
    bool isInFrustum(const glm::vec3& center, float radius) const;

private:
    void updateVectors();
    void updateFrustum();

    CameraMode m_mode = CameraMode::PERSPECTIVE;

    glm::vec3 m_position = glm::vec3(0.0f);
    glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float m_fov = 75.0f;
    float m_aspectRatio = 16.0f / 9.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 1000.0f;

    // Orthographic params
    float m_orthoLeft = -1.0f;
    float m_orthoRight = 1.0f;
    float m_orthoBottom = -1.0f;
    float m_orthoTop = 1.0f;

    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);

    // Frustum planes (left, right, bottom, top, near, far)
    glm::vec4 m_frustumPlanes[6];
};

} // namespace ExperimentRedbear
