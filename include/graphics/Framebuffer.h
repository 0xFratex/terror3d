#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace ExperimentRedbear {

class Framebuffer {
public:
    Framebuffer();
    ~Framebuffer();

    bool create(int width, int height, bool hasDepth = true, bool hasStencil = false);
    void destroy();

    void bind() const;
    void unbind() const;

    void resize(int width, int height);

    GLuint getID() const { return m_fbo; }
    GLuint getColorTexture() const { return m_colorTexture; }
    GLuint getDepthTexture() const { return m_depthTexture; }
    GLuint getDepthBuffer() const { return m_depthBuffer; }

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    bool isValid() const { return m_valid; }

private:
    GLuint m_fbo = 0;
    GLuint m_colorTexture = 0;
    GLuint m_depthTexture = 0;
    GLuint m_depthBuffer = 0;
    GLuint m_stencilBuffer = 0;

    int m_width = 0;
    int m_height = 0;
    bool m_valid = false;
    bool m_hasDepth = true;
    bool m_hasStencil = false;
};

} // namespace ExperimentRedbear
