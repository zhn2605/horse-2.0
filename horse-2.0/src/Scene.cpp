#include "Scene.hpp"

Scene::Scene(GLuint shader) {
	m_shaderProgram = shader;
}

Mesh3D* Scene::CreateObject(const MeshData& data) {
    auto mesh = std::make_unique<Mesh3D>();
    mesh->SpecifyVertices(data.vertices, data.indices);
    mesh->Initialize();

    Mesh3D* ptr = mesh.get();
    m_objects.push_back(std::move(mesh));
    return ptr;
}

void Scene::DrawAll(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(m_shaderProgram);

    // Set view and projection matrices
    GLint viewLocation = glGetUniformLocation(m_shaderProgram, "u_ViewMatrix");
    GLint projLocation = glGetUniformLocation(m_shaderProgram, "u_Projection");

    if (viewLocation >= 0) glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    if (projLocation >= 0) glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);
    for (const auto& obj : m_objects) {
        GLint modelLocation = glGetUniformLocation(m_shaderProgram, "u_ModelMatrix");
        if (modelLocation >= 0) {
            glm::mat4 model = obj->GetModelMatrix();
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);
        }
        // Draw object
        obj->Draw();
    }
}

void Scene::CleanUpAll() {
    for (auto& obj : m_objects) {
        obj->CleanUp();
    }
    m_objects.clear();
}