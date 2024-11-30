#ifndef MESH_DATA_HPP
#define MESH_DATA_HPP

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct MeshData {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    static MeshData CreateCube(float size = 1.0f);
    static MeshData CreateDiamond(float size = 1.0f);
    static MeshData CreatePyramid(float size = 1.0f);
    static MeshData CreateWall(float length = 2.0f, float width = .1f, float height = 6.0f);

};

#endif