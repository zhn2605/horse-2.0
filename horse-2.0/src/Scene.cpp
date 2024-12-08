#include "Scene.hpp"

Scene::Scene(GLuint shader) {
	m_shaderProgram = shader;
}

void Scene::SetShaderProgram(GLuint shader) {
    m_shaderProgram = shader;
}

Mesh3D* Scene::CreateObject(const std::string name, const MeshData& data) {
    auto obj = std::make_unique<Mesh3D>();
    obj->SpecifyVertices(data.vertices, data.indices);
    obj->Initialize();
    obj->SetName(name);

    Mesh3D* ptr = obj.get();
    m_objects.push_back(std::move(obj));
    return ptr;
}

Mesh3D* Scene::GetObject(const std::string name) {
    for (const auto& obj : m_objects) {
        if (obj->GetName() == name) {
            return obj.get();
        }
    }
    
    std::cerr << "Object not found in scene" << std::endl;
    return nullptr;
}

void Scene::PrepareDraw(int width, int height) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, width, height);        // creates a viewport starting left corner (0,0) 
    //glClearColor(0.2f, 0.3f, .3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);        // modifying shaders in program object will not affect curr executables

}

void Scene::DrawObjects(const glm::mat4& view, const glm::mat4& projection, Shader* shader) {
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
        if (obj->IsLightEmitter() == false) {
            obj->Draw(shader);
        }
    }
}

void Scene::DrawLightSources(const glm::mat4& view, const glm::mat4& projection, Shader* lightShader) {
    lightShader->useProgram();
    lightShader->setUniformMat4("u_ViewMatrix", view);
    lightShader->setUniformMat4("u_Projection", projection);

    for (auto& obj : m_objects) {
        if (obj->IsLightEmitter()) {
            glm::mat4 model = obj->GetModelMatrix();
            lightShader->setUniformMat4("u_ModelMatrix", model);

            glm::vec3 lightColor = obj->GetColor();
            lightShader->setUniformVec3("u_LightColor", lightColor);

            obj->Draw(lightShader);
        }
    }
}

void Scene::UpdateAll() {
    for (auto& obj : m_objects) {
        obj->UpdateBuffers();
    }
}

void Scene::CleanUpAll() {
    for (auto& obj : m_objects) {
        obj->CleanUp();
    }
    m_objects.clear();
}