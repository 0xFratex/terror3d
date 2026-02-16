#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>

// FreeType includes
#include <ft2build.h>
#include FT_FREETYPE_H

namespace ExperimentRedbear {

class ShaderProgram;

struct Character {
    GLuint textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    long advance;
};

struct Font {
    std::string name;
    std::string path;
    int size;
    std::unordered_map<char, Character> characters;
    GLuint vao;
    GLuint vbo;
    float lineHeight;
    float base;
};

class TextRenderer {
public:
    static TextRenderer& getInstance();

    bool initialize();
    void shutdown();

    bool loadFont(const std::string& name, const std::string& path, int size);

    void renderText(const std::string& text, const glm::vec2& position, const std::string& font, 
                    float scale, const glm::vec3& color, bool centered = false);

    glm::vec2 measureText(const std::string& text, const std::string& font, float scale);

    float getLineHeight(const std::string& font, float scale);
    
    void setScreenSize(int width, int height) {
        m_screenWidth = width;
        m_screenHeight = height;
    }

private:
    TextRenderer() = default;
    ~TextRenderer() = default;
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;

    FT_Library m_ftLibrary;
    std::unordered_map<std::string, Font> m_fonts;
    std::shared_ptr<ShaderProgram> m_textShader;
    int m_screenWidth = 1920;
    int m_screenHeight = 1080;
    bool m_initialized = false;
};

} // namespace ExperimentRedbear
