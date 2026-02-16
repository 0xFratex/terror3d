#include "ui/TextRenderer.h"
#include "graphics/Shader.h"
#include "core/Logger.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace ExperimentRedbear {

TextRenderer& TextRenderer::getInstance() {
    static TextRenderer instance;
    return instance;
}

bool TextRenderer::initialize() {
    if (FT_Init_FreeType(&m_ftLibrary)) {
        LOG_ERROR("Failed to initialize FreeType");
        return false;
    }

    // Load text shader
    m_textShader = std::make_shared<ShaderProgram>();
    
    auto vertShader = std::make_shared<Shader>();
    auto fragShader = std::make_shared<Shader>();
    
    if (!vertShader->loadFromFile("shaders/text.vert", ShaderType::VERTEX)) {
        LOG_ERROR("Failed to load text vertex shader");
        return false;
    }
    
    if (!fragShader->loadFromFile("shaders/text.frag", ShaderType::FRAGMENT)) {
        LOG_ERROR("Failed to load text fragment shader");
        return false;
    }
    
    m_textShader->attachShader(*vertShader);
    m_textShader->attachShader(*fragShader);
    
    if (!m_textShader->link()) {
        LOG_ERROR("Failed to link text shader program");
        return false;
    }

    m_initialized = true;
    LOG_INFO("Text renderer initialized");
    return true;
}

void TextRenderer::shutdown() {
    for (auto& pair : m_fonts) {
        glDeleteVertexArrays(1, &pair.second.vao);
        glDeleteBuffers(1, &pair.second.vbo);
    }
    m_fonts.clear();
    
    m_textShader.reset();

    if (m_ftLibrary) {
        FT_Done_FreeType(m_ftLibrary);
    }

    m_initialized = false;
}

bool TextRenderer::loadFont(const std::string& name, const std::string& path, int size) {
    Font font;
    font.name = name;
    font.path = path;
    font.size = size;

    FT_Face face;
    if (FT_New_Face(m_ftLibrary, path.c_str(), 0, &face)) {
        LOG_ERROR("Failed to load font: " + path);
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    // Generate VAO and VBO for this font
    glGenVertexArrays(1, &font.vao);
    glGenBuffers(1, &font.vbo);

    glBindVertexArray(font.vao);
    glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load first 128 ASCII characters
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            LOG_WARNING("Failed to load character: " + std::string(1, c));
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character;
        character.textureID = texture;
        character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        character.advance = face->glyph->advance.x;

        font.characters[c] = character;
    }

    FT_Done_Face(face);

    m_fonts[name] = font;
    LOG_INFO("Font loaded: " + name + " (" + path + ")");
    return true;
}

void TextRenderer::renderText(const std::string& text, const glm::vec2& position, 
                               const std::string& font, float scale, 
                               const glm::vec3& color, bool centered) {
    if (m_fonts.find(font) == m_fonts.end()) {
        LOG_WARNING("Font not found: " + font);
        return;
    }
    
    if (!m_textShader) {
        LOG_ERROR("Text shader not initialized");
        return;
    }

    Font& f = m_fonts[font];

    // Calculate centered position if needed
    glm::vec2 pos = position;
    if (centered) {
        glm::vec2 size = measureText(text, font, scale);
        pos.x -= size.x / 2.0f;
    }

    // Bind shader
    m_textShader->bind();
    
    // Set projection matrix (orthographic for 2D text)
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 
                                       0.0f, static_cast<float>(m_screenHeight));
    m_textShader->setMat4("projection", projection);
    m_textShader->setVec3("textColor", color);
    m_textShader->setInt("text", 0);  // Texture unit 0

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(f.vao);

    // Iterate through all characters
    for (const char& c : text) {
        if (f.characters.find(c) == f.characters.end()) continue;

        Character ch = f.characters[c];

        float xpos = pos.x + ch.bearing.x * scale;
        float ypos = pos.y + (ch.bearing.y - ch.size.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos,       0.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f }
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, f.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Advance cursors for next glyph
        pos.x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    m_textShader->unbind();
}

glm::vec2 TextRenderer::measureText(const std::string& text, const std::string& font, float scale) {
    if (m_fonts.find(font) == m_fonts.end()) {
        return glm::vec2(0.0f);
    }

    Font& f = m_fonts[font];
    float width = 0.0f;
    float maxHeight = 0.0f;

    for (const char& c : text) {
        if (f.characters.find(c) == f.characters.end()) continue;

        Character ch = f.characters[c];
        width += (ch.advance >> 6) * scale;
        maxHeight = std::max(maxHeight, ch.size.y * scale);
    }

    return glm::vec2(width, maxHeight);
}

float TextRenderer::getLineHeight(const std::string& font, float scale) {
    if (m_fonts.find(font) == m_fonts.end()) {
        return 0.0f;
    }

    Font& f = m_fonts[font];
    if (f.characters.find('H') != f.characters.end()) {
        return f.characters['H'].size.y * scale;
    }

    return 0.0f;
}

} // namespace ExperimentRedbear
