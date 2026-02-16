#include "core/Timer.h"

namespace ExperimentRedbear {

Timer::Timer() {
    reset();
}

void Timer::reset() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_lastTime = m_startTime;
    m_deltaTime = 0.0f;
    m_totalTime = 0.0f;
    m_fps = 0.0f;
    m_frameTime = 0.0f;
    m_frameCount = 0;
    m_fpsTimer = 0.0f;
}

void Timer::update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    
    m_deltaTime = std::chrono::duration<float>(currentTime - m_lastTime).count();
    m_lastTime = currentTime;
    
    m_totalTime += m_deltaTime;
    m_frameTime = m_deltaTime * 1000.0f; // Convert to milliseconds
    
    m_frameCount++;
    m_fpsTimer += m_deltaTime;
    
    if (m_fpsTimer >= 1.0f) {
        m_fps = static_cast<float>(m_frameCount) / m_fpsTimer;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;
    }
}

float Timer::getCurrentTime() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration<float>(duration).count();
}

} // namespace ExperimentRedbear
