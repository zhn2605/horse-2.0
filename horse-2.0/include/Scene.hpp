#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MeshData.hpp"
#include "Mesh3D.hpp"
#include "Shader.hpp"

class Scene{
public:
	Scene(GLuint shader);

	Mesh3D* CreateObject(const std::string name, const MeshData& data);
	Mesh3D* GetObject(const std::string name);
	void PrepareDraw(int width, int height);
	void DrawObjects(const glm::mat4& view, const glm::mat4& projection, Shader* shader);
	void DrawLightSources(const glm::mat4& view, const glm::mat4& projection, Shader* lightShader);
	void UpdateAll();
	void CleanUpAll();

	void SetShaderProgram(GLuint shader);
private:
	std::string m_name;
	std::vector<std::unique_ptr<Mesh3D>> m_objects;
	GLuint m_shaderProgram;
};


#endif
