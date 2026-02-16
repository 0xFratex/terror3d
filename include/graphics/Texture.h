#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace ExperimentRedbear {

enum class TextureFormat {
    RGB,
    RGBA,
    RED,
    DEPTH,
    DEPTH_STENCIL
};

enum class TextureFilter {
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR
};

enum class TextureWrap {
    REPEAT,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER
};

struct TextureParams {
    TextureFormat format = TextureFormat::RGBA;
    TextureFilter minFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
    TextureFilter magFilter = TextureFilter::LINEAR;
    TextureWrap wrapS = TextureWrap::REPEAT;
    TextureWrap wrapT = TextureWrap::REPEAT;
    bool generateMipmaps = true;
    int anisotropy = 16;
};

class Texture {
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& filepath, const TextureParams& params = TextureParams());
    bool loadFromMemory(const unsigned char* data, int width, int height, const TextureParams& params = TextureParams());
    bool createEmpty(int width, int height, const TextureParams& params = TextureParams());

    void bind(int unit = 0) const;
    void unbind() const;

    GLuint getID() const { return m_textureID; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getChannels() const { return m_channels; }

    void setParameter(GLenum param, GLint value);
    void setParameter(GLenum param, GLfloat value);

    static GLuint createCubemap(const std::vector<std::string>& faces);

private:
    GLuint m_textureID = 0;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
};

} // namespace ExperimentRedbear
