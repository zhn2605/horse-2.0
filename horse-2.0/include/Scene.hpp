#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MeshData.hpp" z
#include "Mesh3D.hpp"

class Scene{
public:
	Scene(GLuint shader);

	Mesh3D* CreateObject(const MeshData& data);
	void PrepareDraw(int width, int height);
	void DrawAll(const glm::mat4& view, const glm::mat4& projection);
	void CleanUpAll();

	void SetShaderProgram(GLuint shader);

private:
	std::string m_name;
	std::vector<std::unique_ptr<Mesh3D>> m_objects;
	GLuint m_shaderProgram;
};


#endif
