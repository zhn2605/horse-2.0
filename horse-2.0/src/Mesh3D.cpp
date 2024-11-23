#include "Mesh3D.hpp"

Mesh3D::Mesh3D() {
}

void Mesh3D::Initialize() {

    // VAO Specification
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Create VBO
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3, // xyz
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 6,
        (void*)0
    );

    // Enable color 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3, // rgb
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 6,
        (void*)(sizeof(GL_FLOAT) * 3)
    );

    // Create EBO
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicies.size() * sizeof(GLuint), m_indicies.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Mesh3D::Draw() {
    // TODO: Implement this .??!?1
}

void Mesh3D::SpecifyVertices(std::vector<GLfloat> vertices, std::vector<GLuint> indicies) {
    m_vertices = vertices;
    m_indicies = indicies;
}

void Mesh3D::CleanUp() {
    if (m_vertexArrayObject != 0) {
        glDeleteBuffers(1, &m_vertexBufferObject);
        m_vertexBufferObject = 0;
    }
    if (m_vertexArrayObject != 0) {
        glDeleteVertexArrays(1, &m_vertexArrayObject);
        m_vertexArrayObject = 0;
    }
    if (m_vertexArrayObject != 0) {
        glDeleteBuffers(1, &m_indexBufferObject);
        m_indexBufferObject = 0;
    }
}