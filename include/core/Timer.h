#pragma once

#include <chrono>

namespace ExperimentRedbear {

class Timer {
public:
    Timer();

    void reset();
    void update();

    float getDeltaTime() const { return m_deltaTime; }
    float getTotalTime() const { return m_totalTime; }
    float getFPS() const { return m_fps; }
    float getFrameTime() const { return m_frameTime; }

    static float getCurrentTime();

private:
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::chrono::high_resolution_clock::time_point m_lastTime;

    float m_deltaTime = 0.0f;
    float m_totalTime = 0.0f;
    float m_fps = 0.0f;
    float m_frameTime = 0.0f;
    int m_frameCount = 0;
    float m_fpsTimer = 0.0f;
};

} // namespace ExperimentRedbear
