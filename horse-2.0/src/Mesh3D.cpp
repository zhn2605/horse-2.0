#include "Mesh3D.hpp"

// Setup functions
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
        sizeof(GL_FLOAT) * 8,  // stride is 8 because [x, y, z, r, g, b, textx, texty]
        (void*)0
    );

    // Enable color 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3, // rgb
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 8,
        (void*)(sizeof(GL_FLOAT) * 3)
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2, // x, y, texture coords
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 8,
        (void*)(sizeof(GL_FLOAT) * 6)
    );

    // Create EBO
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Mesh3D::SpecifyVertices(std::vector<GLfloat> vertices, std::vector<GLuint> indicies) {
    m_vertices = vertices;
    m_indices = indicies;
}

// Render functions
void Mesh3D::Draw() {
    if (m_texture) {
        m_texture->Bind();

    }

    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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
    if (m_indexBufferObject != 0) {
        glDeleteBuffers(1, &m_indexBufferObject);
        m_indexBufferObject = 0;
    }
}

void Mesh3D::UpdateBuffers() {
    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Setters
void Mesh3D::SetTexture(Texture* texture) {
    m_texture = texture;
}

void Mesh3D::SetPosition(const glm::vec3& pos) { 
    m_position = pos;
}
void Mesh3D::SetRotation(float angle, const glm::vec3& axis) {
    m_rotationAngle = angle;
    m_rotationAxis = axis;
}

void Mesh3D::SetScale(const glm::vec3& scale) {
    m_scale = scale;
}

void Mesh3D::SetColor(const glm::vec3& rgb) {
    for (int i = 3; i < m_vertices.size(); i+=8) {
        m_vertices[i] = rgb.r;
        m_vertices[i + 1] = rgb.g;
        m_vertices[i + 2] = rgb.b;
    }
}

void Mesh3D::SetName(const std::string name) {
    m_name = name;
}

//void Mesh3D::Stretch(char axis, int scale) {
//    int startIndex;
//    int step = 6;
//    
//    if (axis == 'X' || axis == 'x') {
//        startIndex = 0;
//    }
//    else if (axis == 'Y' || axis == 'y') {
//        startIndex = 1;
//    } 
//    else if (axis == 'Z' || axis == 'z') {
//        startIndex = 2;
//    }
//
//    
//    for (int i = startIndex; i < m_vertices.size(); i += 3) {
//
//    }
//}

// Getters
glm::mat4 Mesh3D::GetModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, m_rotationAngle, m_rotationAxis);
    model = glm::scale(model, m_scale);
    return model;
}