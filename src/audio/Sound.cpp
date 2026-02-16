#include "audio/Sound.h"
#include "core/Logger.h"
#include <AL/al.h>
#include "stb_vorbis.h"
#include <cstring>

namespace ExperimentRedbear {

Sound::Sound() : m_source(0), m_buffer(0), m_type(SoundType::SFX), m_duration(0.0f), m_loaded(false) {
    alGenSources(1, &m_source);
    alGenBuffers(1, &m_buffer);
}

Sound::~Sound() {
    if (m_source) {
        alDeleteSources(1, &m_source);
    }
    if (m_buffer) {
        alDeleteBuffers(1, &m_buffer);
    }
}

bool Sound::loadFromFile(const std::string& filepath) {
    // Load OGG Vorbis file
    int channels, sampleRate;
    short* data = nullptr;
    int samples = stb_vorbis_decode_filename(filepath.c_str(), &channels, &sampleRate, &data);

    if (samples <= 0 || !data) {
        LOG_ERROR("Failed to load sound file: " + filepath);
        return false;
    }

    // Determine format
    ALenum format;
    if (channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        LOG_ERROR("Unsupported channel count: " + std::to_string(channels));
        free(data);
        return false;
    }

    // Upload to buffer
    alBufferData(m_buffer, format, data, samples * sizeof(short) * channels, sampleRate);
    alSourcei(m_source, AL_BUFFER, m_buffer);

    // Calculate duration
    m_duration = static_cast<float>(samples) / sampleRate;

    free(data);

    m_loaded = true;
    LOG_DEBUG("Sound loaded: " + filepath + " (duration: " + std::to_string(m_duration) + "s)");
    return true;
}

bool Sound::loadFromMemory(const unsigned char* data, size_t size) {
    // Would implement memory loading here
    return false;
}

void Sound::play() {
    if (m_loaded) {
        alSourcePlay(m_source);
    }
}

void Sound::pause() {
    if (m_loaded) {
        alSourcePause(m_source);
    }
}

void Sound::stop() {
    if (m_loaded) {
        alSourceStop(m_source);
    }
}

void Sound::rewind() {
    if (m_loaded) {
        alSourceRewind(m_source);
    }
}

bool Sound::isPlaying() const {
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool Sound::isPaused() const {
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}

bool Sound::isStopped() const {
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

void Sound::setLooping(bool loop) {
    alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void Sound::setVolume(float volume) {
    alSourcef(m_source, AL_GAIN, volume);
}

void Sound::setPitch(float pitch) {
    alSourcef(m_source, AL_PITCH, pitch);
}

void Sound::setPosition(const glm::vec3& position) {
    alSource3f(m_source, AL_POSITION, position.x, position.y, position.z);
}

void Sound::setVelocity(const glm::vec3& velocity) {
    alSource3f(m_source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void Sound::setRelativeToListener(bool relative) {
    alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
}

void Sound::setMinDistance(float distance) {
    alSourcef(m_source, AL_REFERENCE_DISTANCE, distance);
}

void Sound::setMaxDistance(float distance) {
    alSourcef(m_source, AL_MAX_DISTANCE, distance);
}

void Sound::setRolloffFactor(float factor) {
    alSourcef(m_source, AL_ROLLOFF_FACTOR, factor);
}

} // namespace ExperimentRedbear
