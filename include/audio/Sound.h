#pragma once

#include <string>
#include <glm/glm.hpp>
#include <AL/al.h>

namespace ExperimentRedbear {

enum class SoundType {
    SFX,
    MUSIC,
    AMBIENT,
    DIALOGUE
};

class Sound {
public:
    Sound();
    ~Sound();

    bool loadFromFile(const std::string& filepath);
    bool loadFromMemory(const unsigned char* data, size_t size);

    void play();
    void pause();
    void stop();
    void rewind();

    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;

    void setLooping(bool loop);
    void setVolume(float volume);
    void setPitch(float pitch);
    void setPosition(const glm::vec3& position);
    void setVelocity(const glm::vec3& velocity);
    void setRelativeToListener(bool relative);
    void setMinDistance(float distance);
    void setMaxDistance(float distance);
    void setRolloffFactor(float factor);

    ALuint getSource() const { return m_source; }
    ALuint getBuffer() const { return m_buffer; }

    SoundType getType() const { return m_type; }
    void setType(SoundType type) { m_type = type; }

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    float getDuration() const { return m_duration; }

private:
    ALuint m_source = 0;
    ALuint m_buffer = 0;
    SoundType m_type = SoundType::SFX;
    std::string m_name;
    float m_duration = 0.0f;
    bool m_loaded = false;
};

} // namespace ExperimentRedbear
