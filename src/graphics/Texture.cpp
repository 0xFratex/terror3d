#include "graphics/Texture.h"
#include "core/Logger.h"
#include "stb_image.h"

namespace ExperimentRedbear {

Texture::Texture() : m_textureID(0), m_width(0), m_height(0), m_channels(0) {}

Texture::~Texture() {
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
    }
}

bool Texture::loadFromFile(const std::string& filepath, const TextureParams& params) {
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, 0);
    if (!data) {
        LOG_ERROR("Failed to load texture: " + filepath);
        return false;
    }

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB;

    switch (m_channels) {
        case 1:
            format = GL_RED;
            internalFormat = GL_RED;
            break;
        case 2:
            format = GL_RG;
            internalFormat = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            internalFormat = params.generateMipmaps ? GL_SRGB : GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            internalFormat = params.generateMipmaps ? GL_SRGB_ALPHA : GL_RGBA;
            break;
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

    if (params.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(params.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(params.magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(params.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(params.wrapT));

    // Anisotropic filtering
    if (params.anisotropy > 0) {
        GLfloat maxAniso;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 
                        std::min(static_cast<float>(params.anisotropy), maxAniso));
    }

    stbi_image_free(data);

    LOG_DEBUG("Texture loaded: " + filepath + " (" + std::to_string(m_width) + "x" + std::to_string(m_height) + ")");
    return true;
}

bool Texture::loadFromMemory(const unsigned char* data, int width, int height, const TextureParams& params) {
    m_width = width;
    m_height = height;
    m_channels = 4; // Assuming RGBA

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (params.generateMipmaps) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(params.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(params.magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(params.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(params.wrapT));

    return true;
}

bool Texture::createEmpty(int width, int height, const TextureParams& params) {
    m_width = width;
    m_height = height;

    GLenum internalFormat = GL_RGBA;
    GLenum format = GL_RGBA;

    switch (params.format) {
        case TextureFormat::RGB:
            internalFormat = GL_RGB;
            format = GL_RGB;
            break;
        case TextureFormat::RGBA:
            internalFormat = GL_RGBA;
            format = GL_RGBA;
            break;
        case TextureFormat::RED:
            internalFormat = GL_RED;
            format = GL_RED;
            break;
        case TextureFormat::DEPTH:
            internalFormat = GL_DEPTH_COMPONENT;
            format = GL_DEPTH_COMPONENT;
            break;
        case TextureFormat::DEPTH_STENCIL:
            internalFormat = GL_DEPTH24_STENCIL8;
            format = GL_DEPTH_STENCIL;
            break;
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(params.minFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(params.magFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(params.wrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(params.wrapT));

    return true;
}

void Texture::bind(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setParameter(GLenum param, GLint value) {
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, param, value);
}

void Texture::setParameter(GLenum param, GLfloat value) {
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameterf(GL_TEXTURE_2D, param, value);
}

GLuint Texture::createCubemap(const std::vector<std::string>& faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, channels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
        if (data) {
            GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            LOG_ERROR("Cubemap texture failed to load at path: " + faces[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

} // namespace ExperimentRedbear
