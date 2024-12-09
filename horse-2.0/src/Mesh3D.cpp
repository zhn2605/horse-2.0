#include "Mesh3D.hpp"

// Setup functions
Mesh3D::Mesh3D() {
}

// Assimp
bool Mesh3D::LoadModel(const std::string& filepath) {
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filepath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs);

    // Error checking
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error! Assimp: " << importer.GetErrorString() << std::endl;
        return false;
    }

    ProcessNode(scene->mRootNode, scene);
}

void Mesh3D::ProcessNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

void Mesh3D::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Positions
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        // Normals (if available)
        if (mesh->HasNormals()) {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        // Textures (Set first)
        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }

        // Populate already processed vertices
        m_processedVertices.push_back(vertex);
    }

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            m_processedIndices.push_back(face.mIndices[j]);
        }
    }
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
        sizeof(GL_FLOAT) * 11,  // stride is 8 because [x, y, z, r, g, b, textx, texty]
        (void*)0
    );

    // Enable color 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3, // rgb
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 11,
        (void*)(sizeof(GL_FLOAT) * 3)
    );

    // Enable textures
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2, // u, v, texture coords
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 11,
        (void*)(sizeof(GL_FLOAT) * 6)
    );

    // Enable norm attrib
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(GL_FLOAT) * 11,
        (void*)(sizeof(GL_FLOAT) * 8)
    );

    // Create EBO
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void Mesh3D::InitializeModel() {
    // VAO Specification
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    // Create VBO
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_processedVertices.size() * sizeof(Vertex), m_processedVertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,              // attribute location
        3,              // xyz
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex), // stride
        (void*)offsetof(Vertex, Position)
    );

    // Color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,              // attribute location
        3,              // rgb
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal)  // Using Normal as color for now
    );

    // Texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,              // attribute location
        2,              // u, v texture coords
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords)
    );

    // Create EBO
    glGenBuffers(1, &m_indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_processedIndices.size() * sizeof(GLuint), m_processedIndices.data(), GL_STATIC_DRAW);

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
void Mesh3D::Draw(Shader* shader) {
    bool useTexture = (m_texture != nullptr);
    shader->setBool("u_useTexture", useTexture);

    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        m_texture->Bind();
        shader->setInt("textureSampler", 0);
    } 

    // Handlle object color
    shader->setUniformVec3("u_objectColor", m_color);

    // Draw Mesh
    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (useTexture) {
        m_texture->Unbind();
    }
}


void Mesh3D::DrawModel(Shader* shader) {
    bool useTexture = (m_texture != nullptr);
    shader->setBool("u_useTexture", useTexture);

    if (useTexture) {
        glActiveTexture(GL_TEXTURE0);
        m_texture->Bind();
        shader->setInt("textureSampler", 0);
    }

    // Handlle object color
    shader->setUniformVec3("u_objectColor", m_color);

    // Draw Model
    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferObject);
    glDrawElements(GL_TRIANGLES, m_processedIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    if (useTexture) {
        m_texture->Unbind();
    }
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

    if (!m_processedVertices.empty()) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_processedVertices.size() * sizeof(float), m_processedVertices.data());
    }
    else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(float), m_vertices.data());
    }

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
    m_color = rgb;
    
    for (int i = 3; i < m_vertices.size(); i+=11) {
        m_vertices[i] = m_color.r;
        m_vertices[i + 1] = m_color.g;
        m_vertices[i + 2] = m_color.b;
    }
}

void Mesh3D::SetName(const std::string name) {
    m_name = name;
}

void Mesh3D::SetLightEmitter(bool isLightEmitter) {
    m_isLightEmitter = isLightEmitter;
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