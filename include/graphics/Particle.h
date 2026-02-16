#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

namespace ExperimentRedbear {

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float size;
    float life;
    float maxLife;
};

class ParticleSystem {
public:
    ParticleSystem();
    ~ParticleSystem();

    void initialize();
    void update(float deltaTime);
    void render();

    void emit(const Particle& particle);
    void emitBurst(int count, const glm::vec3& position, const glm::vec4& color, 
                   float minSpeed, float maxSpeed, float minLife, float maxLife);

    void setMaxParticles(int max) { m_maxParticles = max; }
    int getParticleCount() const { return static_cast<int>(m_particles.size()); }

private:
    std::vector<Particle> m_particles;
    int m_maxParticles = 10000;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_texture = 0;

    bool m_initialized = false;
};

} // namespace ExperimentRedbear
