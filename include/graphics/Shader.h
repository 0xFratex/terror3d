#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace ExperimentRedbear {

enum class ShaderType {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    COMPUTE = GL_COMPUTE_SHADER
};

class Shader {
public:
    Shader();
    ~Shader();

    bool loadFromFile(const std::string& filepath, ShaderType type);
    bool loadFromSource(const std::string& source, ShaderType type);

    GLuint getID() const { return m_shaderID; }
    ShaderType getType() const { return m_type; }

    bool isCompiled() const { return m_compiled; }
    std::string getCompileLog() const { return m_compileLog; }

private:
    bool compile();

    GLuint m_shaderID = 0;
    ShaderType m_type;
    bool m_compiled = false;
    std::string m_compileLog;
};

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    bool attachShader(const Shader& shader);
    bool link();

    void bind() const;
    void unbind() const;

    // Uniform setters
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setBool(const std::string& name, bool value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat3(const std::string& name, const glm::mat3& value);
    void setMat4(const std::string& name, const glm::mat4& value);

    void setInt(const std::string& name, int count, const int* values);
    void setFloat(const std::string& name, int count, const float* values);
    void setVec3(const std::string& name, int count, const float* values);
    void setMat4(const std::string& name, int count, const float* values);

    GLuint getID() const { return m_programID; }
    bool isLinked() const { return m_linked; }
    std::string getLinkLog() const { return m_linkLog; }

private:
    GLint getUniformLocation(const std::string& name);

    GLuint m_programID = 0;
    bool m_linked = false;
    std::string m_linkLog;
    std::unordered_map<std::string, GLint> m_uniformLocationCache;
};

} // namespace ExperimentRedbear
