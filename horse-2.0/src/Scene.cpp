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

void Scene::DrawAll() {
    for (const auto& obj : m_objects) {
        obj->Draw(); // TODO: Implement draw fcn in obj.
    }
}

void Scene::CleanUpAll() {
    for (auto& obj : m_objects) {
        obj->CleanUp();
    }
    m_objects.clear();
}