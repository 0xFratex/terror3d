#include "audio/AudioManager.h"
#include "core/Logger.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace ExperimentRedbear {

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

bool AudioManager::initialize() {
    // Open default device
    m_device = alcOpenDevice(nullptr);
    if (!m_device) {
        LOG_ERROR("Failed to open audio device");
        return false;
    }

    // Create context
    m_context = alcCreateContext(m_device, nullptr);
    if (!m_context) {
        LOG_ERROR("Failed to create audio context");
        alcCloseDevice(m_device);
        return false;
    }

    if (!alcMakeContextCurrent(m_context)) {
        LOG_ERROR("Failed to make audio context current");
        alcDestroyContext(m_context);
        alcCloseDevice(m_device);
        return false;
    }

    // Set default listener values
    setListenerPosition(glm::vec3(0.0f));
    setListenerOrientation(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    setListenerVelocity(glm::vec3(0.0f));

    m_initialized = true;
    LOG_INFO("Audio manager initialized");
    return true;
}

void AudioManager::shutdown() {
    m_sounds.clear();
    m_currentAmbience.reset();

    if (m_context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_context);
        m_context = nullptr;
    }

    if (m_device) {
        alcCloseDevice(m_device);
        m_device = nullptr;
    }

    m_initialized = false;
    LOG_INFO("Audio manager shut down");
}

void AudioManager::update() {
    // Update streaming sounds, handle callbacks, etc.
}

void AudioManager::setListenerPosition(const glm::vec3& position) {
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioManager::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
    float orientation[6] = {
        forward.x, forward.y, forward.z,
        up.x, up.y, up.z
    };
    alListenerfv(AL_ORIENTATION, orientation);
}

void AudioManager::setListenerVelocity(const glm::vec3& velocity) {
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

std::shared_ptr<Sound> AudioManager::loadSound(const std::string& name, const std::string& filepath) {
    auto sound = std::make_shared<Sound>();
    if (sound->loadFromFile(filepath)) {
        sound->setName(name);
        m_sounds[name] = sound;
        LOG_DEBUG("Sound loaded: " + name);
        return sound;
    }
    return nullptr;
}

std::shared_ptr<Sound> AudioManager::getSound(const std::string& name) {
    auto it = m_sounds.find(name);
    if (it != m_sounds.end()) {
        return it->second;
    }
    return nullptr;
}

void AudioManager::play(const std::string& name) {
    auto sound = getSound(name);
    if (sound) {
        sound->play();
    }
}

void AudioManager::playAt(const std::string& name, const glm::vec3& position) {
    auto sound = getSound(name);
    if (sound) {
        sound->setPosition(position);
        sound->setRelativeToListener(false);
        sound->play();
    }
}

void AudioManager::stop(const std::string& name) {
    auto sound = getSound(name);
    if (sound) {
        sound->stop();
    }
}

void AudioManager::stopAll() {
    for (auto& pair : m_sounds) {
        pair.second->stop();
    }
}

void AudioManager::setMasterVolume(float volume) {
    m_settings.masterVolume = volume;
    alListenerf(AL_GAIN, volume);
}

void AudioManager::setMusicVolume(float volume) {
    m_settings.musicVolume = volume;
}

void AudioManager::setSFXVolume(float volume) {
    m_settings.sfxVolume = volume;
}

void AudioManager::setAmbientVolume(float volume) {
    m_settings.ambientVolume = volume;
}

void AudioManager::playAmbience(const std::string& name, bool loop) {
    auto sound = getSound(name);
    if (sound) {
        if (m_currentAmbience) {
            m_currentAmbience->stop();
        }
        sound->setLooping(loop);
        sound->setRelativeToListener(true);
        sound->play();
        m_currentAmbience = sound;
    }
}

void AudioManager::stopAmbience() {
    if (m_currentAmbience) {
        m_currentAmbience->stop();
        m_currentAmbience.reset();
    }
}

} // namespace ExperimentRedbear
