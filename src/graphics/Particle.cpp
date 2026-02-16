#include "graphics/Particle.h"
#include <GL/glew.h>

namespace ExperimentRedbear {

ParticleSystem::ParticleSystem() {}

ParticleSystem::~ParticleSystem() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_texture) glDeleteTextures(1, &m_texture);
}

void ParticleSystem::initialize() {
    m_particles.reserve(m_maxParticles);
    m_initialized = true;
}

void ParticleSystem::update(float deltaTime) {
    for (auto it = m_particles.begin(); it != m_particles.end(); ) {
        it->life -= deltaTime;
        if (it->life <= 0.0f) {
            it = m_particles.erase(it);
        } else {
            it->position += it->velocity * deltaTime;
            it->velocity.y -= 9.8f * deltaTime; // Gravity
            ++it;
        }
    }
}

void ParticleSystem::render() {
    // Particle rendering implementation
}

void ParticleSystem::emit(const Particle& particle) {
    if (static_cast<int>(m_particles.size()) < m_maxParticles) {
        m_particles.push_back(particle);
    }
}

void ParticleSystem::emitBurst(int count, const glm::vec3& position, const glm::vec4& color,
                                float minSpeed, float maxSpeed, float minLife, float maxLife) {
    for (int i = 0; i < count; i++) {
        if (static_cast<int>(m_particles.size()) >= m_maxParticles) break;
        
        Particle p;
        p.position = position;
        p.color = color;
        p.life = minLife + (maxLife - minLife) * (rand() / float(RAND_MAX));
        p.maxLife = p.life;
        p.size = 0.1f;
        
        // Random direction
        float theta = (rand() / float(RAND_MAX)) * 6.28318f;
        float phi = (rand() / float(RAND_MAX)) * 3.14159f;
        float speed = minSpeed + (maxSpeed - minSpeed) * (rand() / float(RAND_MAX));
        
        p.velocity = glm::vec3(
            sin(phi) * cos(theta),
            cos(phi),
            sin(phi) * sin(theta)
        ) * speed;
        
        m_particles.push_back(p);
    }
}

} // namespace ExperimentRedbear
