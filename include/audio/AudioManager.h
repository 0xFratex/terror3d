#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>
#include <AL/al.h>
#include <AL/alc.h>

namespace ExperimentRedbear {

class Sound;

struct AudioSettings {
    float masterVolume = 1.0f;
    float musicVolume = 0.7f;
    float sfxVolume = 0.8f;
    float ambientVolume = 0.6f;
};

class AudioManager {
public:
    static AudioManager& getInstance();

    bool initialize();
    void shutdown();

    void update();

    // Listener
    void setListenerPosition(const glm::vec3& position);
    void setListenerOrientation(const glm::vec3& forward, const glm::vec3& up);
    void setListenerVelocity(const glm::vec3& velocity);

    // Sound management
    std::shared_ptr<Sound> loadSound(const std::string& name, const std::string& filepath);
    std::shared_ptr<Sound> getSound(const std::string& name);

    // Playback
    void play(const std::string& name);
    void playAt(const std::string& name, const glm::vec3& position);
    void stop(const std::string& name);
    void stopAll();

    // Volume
    void setMasterVolume(float volume);
    void setMusicVolume(float volume);
    void setSFXVolume(float volume);
    void setAmbientVolume(float volume);

    // Ambience
    void playAmbience(const std::string& name, bool loop = true);
    void stopAmbience();
    void setAmbienceFadeTime(float seconds) { m_ambienceFadeTime = seconds; }

    const AudioSettings& getSettings() const { return m_settings; }

private:
    AudioManager() = default;
    ~AudioManager() = default;
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    ALCdevice* m_device = nullptr;
    ALCcontext* m_context = nullptr;

    std::unordered_map<std::string, std::shared_ptr<Sound>> m_sounds;
    std::shared_ptr<Sound> m_currentAmbience;
    float m_ambienceFadeTime = 1.0f;

    AudioSettings m_settings;
    bool m_initialized = false;
};

} // namespace ExperimentRedbear
