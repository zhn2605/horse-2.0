#ifndef MESH3D_H
#define MESH3D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class Mesh3D {
public:
    Mesh3D();

    void SpecifyVertices(std::vector<GLfloat> vertices, std::vector<GLuint> indicies);
    void Initialize();
    void Draw();
    void CleanUp();
    void PrepareDraw();

    // Setters
    void SetPosition(const glm::vec3& pos);
    void SetRotation(float angle, const glm::vec3& axis);
    void SetScale(const glm::vec3& scale);

    // Getters
    glm::mat4 GetModelMatrix() const;
    GLuint getVAO() const { return m_vertexArrayObject; }
    GLuint getVBO() const { return m_vertexBufferObject; }
    GLuint getIBO() const { return m_indexBufferObject; }
private:
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vertexArrayObject = 0;
    GLuint m_vertexBufferObject = 0;
    GLuint m_indexBufferObject = 0;

    // Object Data
    glm::vec3 m_position{ 0.0f };
    float m_rotationAngle{ 0.0f };
    glm::vec3 m_rotationAxis{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_scale{ 1.0f };
};

#endif