#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct MeshData {
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    static MeshData CreateCube(float size = 1.0f);
    static MeshData CreateDiamond(float size = 1.0f);
    static MeshData CreatePyramid(float size = 1.0f);
};