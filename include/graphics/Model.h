#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace ExperimentRedbear {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    GLuint id = 0;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    void create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, 
                const std::vector<Texture>& textures);
    void draw() const;
    void drawInstanced(int count) const;

    GLuint getVAO() const { return m_vao; }
    size_t getIndexCount() const { return m_indices.size(); }
    size_t getVertexCount() const { return m_vertices.size(); }

    const std::vector<Texture>& getTextures() const { return m_textures; }

private:
    void setupMesh();

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
};

class Model {
public:
    Model();
    ~Model();

    bool loadFromFile(const std::string& filepath);
    void draw() const;
    void drawInstanced(int count) const;

    const std::vector<std::unique_ptr<Mesh>>& getMeshes() const { return m_meshes; }
    const glm::vec3& getMinBounds() const { return m_minBounds; }
    const glm::vec3& getMaxBounds() const { return m_maxBounds; }
    glm::vec3 getCenter() const;
    glm::vec3 getSize() const;

private:
    std::vector<std::unique_ptr<Mesh>> m_meshes;
    glm::vec3 m_minBounds = glm::vec3(0.0f);
    glm::vec3 m_maxBounds = glm::vec3(0.0f);
};

} // namespace ExperimentRedbear
