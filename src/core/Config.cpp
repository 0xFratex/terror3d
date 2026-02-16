#include "core/Config.h"
#include "core/Logger.h"
#include <fstream>
#include <sstream>

namespace ExperimentRedbear {

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

bool Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        LOG_WARNING("Could not open config file: " + filename);
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;

        std::string key = line.substr(0, eqPos);
        std::string value = line.substr(eqPos + 1);

        // Trim whitespace
        key = key.substr(key.find_first_not_of(" \t"), key.find_last_not_of(" \t") + 1);
        value = value.substr(value.find_first_not_of(" \t"), value.find_last_not_of(" \t") + 1);

        // Detect type and store
        if (value == "true" || value == "false") {
            m_values[key] = (value == "true");
        } else if (value.find('.') != std::string::npos) {
            m_values[key] = std::stof(value);
        } else if (value.find(',') != std::string::npos) {
            // Vector value (e.g., "1.0, 2.0, 3.0")
            std::stringstream ss(value);
            std::string component;
            std::vector<float> components;
            while (std::getline(ss, component, ',')) {
                components.push_back(std::stof(component));
            }
            if (components.size() == 2) {
                m_values[key] = glm::vec2(components[0], components[1]);
            } else if (components.size() == 3) {
                m_values[key] = glm::vec3(components[0], components[1], components[2]);
            } else if (components.size() == 4) {
                m_values[key] = glm::vec4(components[0], components[1], components[2], components[3]);
            }
        } else {
            // Try to parse as int first, then as string
            try {
                m_values[key] = std::stoi(value);
            } catch (...) {
                m_values[key] = value;
            }
        }
    }

    // Apply loaded settings
    graphics.screenWidth = getInt("screen_width", graphics.screenWidth);
    graphics.screenHeight = getInt("screen_height", graphics.screenHeight);
    graphics.fullscreen = getBool("fullscreen", graphics.fullscreen);
    graphics.vsync = getBool("vsync", graphics.vsync);
    graphics.shadowQuality = getInt("shadow_quality", graphics.shadowQuality);
    graphics.textureQuality = getInt("texture_quality", graphics.textureQuality);
    graphics.renderDistance = getFloat("render_distance", graphics.renderDistance);
    graphics.ssao = getBool("ssao", graphics.ssao);
    graphics.bloom = getBool("bloom", graphics.bloom);
    graphics.msaaSamples = getInt("msaa_samples", graphics.msaaSamples);

    audio.masterVolume = getFloat("master_volume", audio.masterVolume);
    audio.musicVolume = getFloat("music_volume", audio.musicVolume);
    audio.sfxVolume = getFloat("sfx_volume", audio.sfxVolume);
    audio.ambientVolume = getFloat("ambient_volume", audio.ambientVolume);

    gameplay.mouseSensitivity = getFloat("mouse_sensitivity", gameplay.mouseSensitivity);
    gameplay.invertY = getBool("invert_y", gameplay.invertY);
    gameplay.fov = getFloat("fov", gameplay.fov);
    gameplay.showFPS = getBool("show_fps", gameplay.showFPS);

    LOG_INFO("Config loaded from: " + filename);
    return true;
}

bool Config::save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        LOG_ERROR("Could not save config file: " + filename);
        return false;
    }

    file << "# Experiment Redbear Configuration\n";
    file << "# Auto-generated, do not edit manually\n\n";

    file << "# Graphics\n";
    file << "screen_width=" << graphics.screenWidth << "\n";
    file << "screen_height=" << graphics.screenHeight << "\n";
    file << "fullscreen=" << (graphics.fullscreen ? "true" : "false") << "\n";
    file << "vsync=" << (graphics.vsync ? "true" : "false") << "\n";
    file << "shadow_quality=" << graphics.shadowQuality << "\n";
    file << "texture_quality=" << graphics.textureQuality << "\n";
    file << "render_distance=" << graphics.renderDistance << "\n";
    file << "ssao=" << (graphics.ssao ? "true" : "false") << "\n";
    file << "bloom=" << (graphics.bloom ? "true" : "false") << "\n";
    file << "msaa_samples=" << graphics.msaaSamples << "\n\n";

    file << "# Audio\n";
    file << "master_volume=" << audio.masterVolume << "\n";
    file << "music_volume=" << audio.musicVolume << "\n";
    file << "sfx_volume=" << audio.sfxVolume << "\n";
    file << "ambient_volume=" << audio.ambientVolume << "\n\n";

    file << "# Gameplay\n";
    file << "mouse_sensitivity=" << gameplay.mouseSensitivity << "\n";
    file << "invert_y=" << (gameplay.invertY ? "true" : "false") << "\n";
    file << "fov=" << gameplay.fov << "\n";
    file << "show_fps=" << (gameplay.showFPS ? "true" : "false") << "\n";

    LOG_INFO("Config saved to: " + filename);
    return true;
}

void Config::setInt(const std::string& key, int value) {
    m_values[key] = value;
}

void Config::setFloat(const std::string& key, float value) {
    m_values[key] = value;
}

void Config::setBool(const std::string& key, bool value) {
    m_values[key] = value;
}

void Config::setString(const std::string& key, const std::string& value) {
    m_values[key] = value;
}

void Config::setVec2(const std::string& key, const glm::vec2& value) {
    m_values[key] = value;
}

void Config::setVec3(const std::string& key, const glm::vec3& value) {
    m_values[key] = value;
}

void Config::setVec4(const std::string& key, const glm::vec4& value) {
    m_values[key] = value;
}

int Config::getInt(const std::string& key, int defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        if (std::holds_alternative<int>(it->second)) {
            return std::get<int>(it->second);
        }
    }
    return defaultValue;
}

float Config::getFloat(const std::string& key, float defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        if (std::holds_alternative<float>(it->second)) {
            return std::get<float>(it->second);
        }
    }
    return defaultValue;
}

bool Config::getBool(const std::string& key, bool defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        if (std::holds_alternative<bool>(it->second)) {
            return std::get<bool>(it->second);
        }
    }
    return defaultValue;
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        if (std::holds_alternative<std::string>(it->second)) {
            return std::get<std::string>(it->second);
        }
    }
    return defaultValue;
}

glm::vec2 Config::getVec2(const std::string& key, const glm::vec2& defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        if (std::holds_alternative<glm::vec2>(it->second)) {
            return std::get<glm::vec2>(it->second);
        }
    }
    return defaultValue;
}

glm::vec3 Config::getVec3(const std::string& key, const glm::vec3& defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        if (std::holds_alternative<glm::vec3>(it->second)) {
            return std::get<glm::vec3>(it->second);
        }
    }
    return defaultValue;
}

glm::vec4 Config::getVec4(const std::string& key, const glm::vec4& defaultValue) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        if (std::holds_alternative<glm::vec4>(it->second)) {
            return std::get<glm::vec4>(it->second);
        }
    }
    return defaultValue;
}

bool Config::hasKey(const std::string& key) const {
    return m_values.find(key) != m_values.end();
}

void Config::applySettings() {
    // This would apply the loaded settings to the game systems
    LOG_INFO("Applying configuration settings...");
}

} // namespace ExperimentRedbear
