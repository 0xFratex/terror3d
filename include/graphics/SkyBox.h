#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace ExperimentRedbear {

class SkyBox {
public:
    SkyBox();
    ~SkyBox();

    bool loadFromFiles(const std::string& directory, 
                       const std::string& posXTex, const std::string& negXTex,
                       const std::string& posYTex, const std::string& negYTex,
                       const std::string& posZTex, const std::string& negZTex);

    void render(const glm::mat4& view, const glm::mat4& projection);

    GLuint getCubemapTexture() const { return m_cubemapTexture; }

private:
    GLuint m_cubemapTexture = 0;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;

    bool m_loaded = false;
};

} // namespace ExperimentRedbear
