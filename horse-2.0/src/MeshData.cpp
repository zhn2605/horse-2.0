#include "MeshData.hpp"

/*
MeshData MeshData::CreateDiamond(float size) {
    float halfSize = size / 2.0f;
    MeshData data;

    data.vertices = {
        // Front face
        -halfSize,  halfSize,  halfSize,  0.0f, 0.0f, 1.0f,  // top-left
         halfSize,  halfSize,  halfSize,  0.0f, 1.0f, 0.0f,  // top-right
         halfSize, -halfSize,  halfSize,  0.0f, 1.0f, 1.0f,  // bottom-right
        -halfSize, -halfSize,  halfSize,  1.0f, 0.0f, 0.0f,  // bottom-left
        // Back face
        -halfSize,  halfSize, -halfSize,  1.0f, 0.0f, 1.0f,
         halfSize,  halfSize, -halfSize,  0.0f, 1.0f, 1.0f,
         halfSize, -halfSize, -halfSize,  1.0f, 1.0f, 0.0f,
        -halfSize, -halfSize, -halfSize,  1.0f, 1.0f, 1.0f
    };

    data.indices = {
        0, 1, 2, 0, 2, 3,   // Front
        4, 5, 6, 4, 6, 7,   // Back
        4, 0, 3,  4, 3, 7,  // Left
        1, 5, 6,  1, 6, 2,  // Right
        4, 5, 1,  4, 1, 0,  // Top
        3, 2, 6,  3, 6, 7   // Bottom
    };

    return data;
}
*/

MeshData MeshData::CreateCube(float size) {
    float halfSize = size / 2.0f;
    MeshData data;

    data.vertices = {
        // Front face (position, color, texcoords)
        -halfSize,  halfSize,  halfSize,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,      // Top-left
         halfSize,  halfSize,  halfSize,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,      // Top-right
         halfSize, -halfSize,  halfSize,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,      // Bottom-right
        -halfSize, -halfSize,  halfSize,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f,      // Bottom-left

        // Back face
        -halfSize,  halfSize, -halfSize,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,      // Top-left
         halfSize,  halfSize, -halfSize,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // Top-right
         halfSize, -halfSize, -halfSize,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,      // Bottom-right
        -halfSize, -halfSize, -halfSize,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // Bottom-left

        // Left side face
        -halfSize,  halfSize,  halfSize,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,      // Top-left
        -halfSize,  halfSize, -halfSize,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // Top-right
        -halfSize, -halfSize, -halfSize,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,      // Bottom-right
        -halfSize, -halfSize,  halfSize,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // Bottom-left

        // Right side face
         halfSize,  halfSize,  halfSize,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,      // Top-left
         halfSize,  halfSize, -halfSize,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,      // Top-right
         halfSize, -halfSize, -halfSize,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,      // Bottom-right
         halfSize, -halfSize,  halfSize,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f,      // Bottom-left

         // Top face
         -halfSize,  halfSize, -halfSize,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
          halfSize,  halfSize, -halfSize,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
          halfSize,  halfSize,  halfSize,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
         -halfSize,  halfSize,  halfSize,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

         // Bottom face
         -halfSize, -halfSize,  halfSize,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
          halfSize, -halfSize,  halfSize,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
          halfSize, -halfSize, -halfSize,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         -halfSize, -halfSize, -halfSize,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    data.indices = {
        // Front face
        0, 1, 2,
        0, 2, 3,
        // Back face
        4, 5, 6,
        4, 6, 7,
        // Left face
        8, 9, 10,
        8, 10, 11,
        // Right face
        12, 13, 14,
        12, 14, 15,
        // Top face
        16, 17, 18,
        16, 18, 19,
        // Bottom face
        20, 21, 22,
        20, 22, 23
    };

    return data;
}

/*
MeshData MeshData::CreateWall(float length, float width, float height) {
    MeshData data;

    data.vertices = {
        // Front face
        -length/2, 0.0f,  0.0f,  0.85f, 0.85f, 0.91f,
         length/2, 0.0f,  0.0f,  0.85f, 0.85f, 0.85f,
         length/2, height,  0.0f,  0.85f, 0.85f, 0.91f,
        -length/2, height,  0.0f,  0.85f, 0.85f, 0.85f,
        // Back face
        -length/2,  0.0f, -width,  0.85f, 0.85f, 0.91f,
         length/2,  0.0f, -width,  0.85f, 0.85f, 0.85f,
         length/2, height, -width,  0.85f, 0.85f, 0.91f,
        -length/2, height, -width,  0.85f, 0.85f, 0.85f
    }; 

    data.indices = {
        0, 1, 2,  0, 3, 2,  // Front
        4, 5, 6,  4, 7, 6,  // Back
        0, 4, 7,  0, 3, 7,  // Left
        1, 5, 6,  1, 2, 6,  // Right
        0, 4, 5,  0, 1, 5,  // Top
        3, 7, 6,  3, 2, 6   // Bottom
    };

    return data;
}
*/