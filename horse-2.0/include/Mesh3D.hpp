#ifndef MESH3D_H
#define MESH3D_H

#include <glad/glad.h>
#include <glm/glm.hpp>
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

    GLuint getVAO() const { return m_vertexArrayObject; }
    GLuint getVBO() const { return m_vertexBufferObject; }
    GLuint getIBO() const { return m_indexBufferObject; }
private:
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indicies;
    GLuint m_vertexArrayObject = 0;
    GLuint m_vertexBufferObject = 0;
    GLuint m_indexBufferObject = 0;
};

#endif