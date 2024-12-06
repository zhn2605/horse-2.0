#ifndef MESH3D_H
#define MESH3D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cctype>
#include <string>

#include "Texture.hpp"
#include "Shader.hpp"

class Mesh3D {
public:
    Mesh3D();

    void SpecifyVertices(std::vector<GLfloat> vertices, std::vector<GLuint> indicies);
    void Initialize();
    void Draw(Shader* shader);
    void CleanUp();

    void UpdateBuffers();

    // Setters
    void SetTexture(Texture* texture);
    void SetPosition(const glm::vec3& pos);
    void SetRotation(float angle, const glm::vec3& axis);
    void SetScale(const glm::vec3& scale);
    void SetColor(const glm::vec3& color);
    void SetName(const std::string name);
    void SetLightEmitter(bool isLightEmitter);
    void Stretch(char axis, int scale);

    // Getters
    std::string GetName() const { return m_name; }
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetColor() const { return m_color; }
    bool IsLightEmitter() const { return m_isLightEmitter; }

    glm::mat4 GetModelMatrix() const;
    GLuint getVAO() const { return m_vertexArrayObject; }
    GLuint getVBO() const { return m_vertexBufferObject; }
    GLuint getIBO() const { return m_indexBufferObject; }
private:
    Texture* m_texture = nullptr;

    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_vertexArrayObject = 0;
    GLuint m_vertexBufferObject = 0;
    GLuint m_indexBufferObject = 0;

    // Object Data
    std::string m_name = "object";
    glm::vec3 m_position{ 0.0f };
    glm::vec3 m_color{ 1.0f };
    float m_rotationAngle{ 0.0f };
    glm::vec3 m_rotationAxis{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_scale{ 1.0f };
    bool m_isLightEmitter = false;
};

#endif