#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <glm/glm.hpp>

namespace ExperimentRedbear {

using ConfigValue = std::variant<int, float, bool, std::string, glm::vec2, glm::vec3, glm::vec4>;

class Config {
public:
    static Config& getInstance();

    bool load(const std::string& filename);
    bool save(const std::string& filename);

    void setInt(const std::string& key, int value);
    void setFloat(const std::string& key, float value);
    void setBool(const std::string& key, bool value);
    void setString(const std::string& key, const std::string& value);
    void setVec2(const std::string& key, const glm::vec2& value);
    void setVec3(const std::string& key, const glm::vec3& value);
    void setVec4(const std::string& key, const glm::vec4& value);

    int getInt(const std::string& key, int defaultValue = 0) const;
    float getFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    glm::vec2 getVec2(const std::string& key, const glm::vec2& defaultValue = glm::vec2(0.0f)) const;
    glm::vec3 getVec3(const std::string& key, const glm::vec3& defaultValue = glm::vec3(0.0f)) const;
    glm::vec4 getVec4(const std::string& key, const glm::vec4& defaultValue = glm::vec4(0.0f)) const;

    bool hasKey(const std::string& key) const;

    // Graphics settings
    struct GraphicsSettings {
        int screenWidth = 1920;
        int screenHeight = 1080;
        bool fullscreen = false;
        bool vsync = true;
        int shadowQuality = 2; // 0=off, 1=low, 2=medium, 3=high
        int textureQuality = 2;
        float renderDistance = 500.0f;
        bool ssao = true;
        bool bloom = true;
        int msaaSamples = 4;
    };

    // Audio settings
    struct AudioSettings {
        float masterVolume = 1.0f;
        float musicVolume = 0.7f;
        float sfxVolume = 0.8f;
        float ambientVolume = 0.6f;
    };

    // Gameplay settings
    struct GameplaySettings {
        float mouseSensitivity = 0.1f;
        bool invertY = false;
        float fov = 75.0f;
        bool showFPS = false;
    };

    GraphicsSettings graphics;
    AudioSettings audio;
    GameplaySettings gameplay;

    void applySettings();

private:
    Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    std::unordered_map<std::string, ConfigValue> m_values;
};

} // namespace ExperimentRedbear
