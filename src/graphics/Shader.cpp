#include "graphics/Shader.h"
#include "core/Logger.h"
#include <fstream>
#include <sstream>

namespace ExperimentRedbear {

Shader::Shader() : m_shaderID(0), m_compiled(false) {}

Shader::~Shader() {
    if (m_shaderID) {
        glDeleteShader(m_shaderID);
    }
}

bool Shader::loadFromFile(const std::string& filepath, ShaderType type) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        m_compileLog = "Failed to open file: " + filepath;
        LOG_ERROR(m_compileLog);
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return loadFromSource(buffer.str(), type);
}

bool Shader::loadFromSource(const std::string& source, ShaderType type) {
    m_type = type;
    m_shaderID = glCreateShader(static_cast<GLenum>(type));

    const char* src = source.c_str();
    glShaderSource(m_shaderID, 1, &src, nullptr);

    return compile();
}

bool Shader::compile() {
    glCompileShader(m_shaderID);

    GLint success;
    glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint logLength;
        glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &logLength);
        m_compileLog.resize(logLength);
        glGetShaderInfoLog(m_shaderID, logLength, nullptr, &m_compileLog[0]);
        LOG_ERROR("Shader compilation failed: " + m_compileLog);
        m_compiled = false;
        return false;
    }

    m_compiled = true;
    return true;
}

// ShaderProgram implementation

ShaderProgram::ShaderProgram() : m_programID(0), m_linked(false) {
    m_programID = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    if (m_programID) {
        glDeleteProgram(m_programID);
    }
}

bool ShaderProgram::attachShader(const Shader& shader) {
    if (!shader.isCompiled()) {
        LOG_ERROR("Cannot attach uncompiled shader");
        return false;
    }

    glAttachShader(m_programID, shader.getID());
    return true;
}

bool ShaderProgram::link() {
    glLinkProgram(m_programID);

    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

    if (!success) {
        GLint logLength;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLength);
        m_linkLog.resize(logLength);
        glGetProgramInfoLog(m_programID, logLength, nullptr, &m_linkLog[0]);
        LOG_ERROR("Shader program linking failed: " + m_linkLog);
        m_linked = false;
        return false;
    }

    m_linked = true;
    m_uniformLocationCache.clear();
    LOG_DEBUG("Shader program linked successfully");
    return true;
}

void ShaderProgram::bind() const {
    glUseProgram(m_programID);
}

void ShaderProgram::unbind() const {
    glUseProgram(0);
}

GLint ShaderProgram::getUniformLocation(const std::string& name) {
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end()) {
        return it->second;
    }

    GLint location = glGetUniformLocation(m_programID, name.c_str());
    m_uniformLocationCache[name] = location;

    if (location == -1) {
        LOG_WARNING("Uniform not found: " + name);
    }

    return location;
}

void ShaderProgram::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setBool(const std::string& name, bool value) {
    glUniform1i(getUniformLocation(name), value ? 1 : 0);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& value) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setInt(const std::string& name, int count, const int* values) {
    glUniform1iv(getUniformLocation(name), count, values);
}

void ShaderProgram::setFloat(const std::string& name, int count, const float* values) {
    glUniform1fv(getUniformLocation(name), count, values);
}

void ShaderProgram::setVec3(const std::string& name, int count, const float* values) {
    glUniform3fv(getUniformLocation(name), count, values);
}

void ShaderProgram::setMat4(const std::string& name, int count, const float* values) {
    glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, values);
}

} // namespace ExperimentRedbear
