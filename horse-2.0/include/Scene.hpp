#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MeshData.hpp"
#include "Mesh3D.hpp"

class Scene{
public:
	Scene(GLuint shader);

	Mesh3D* CreateObject(const MeshData& data);
	void DrawAll(const glm::mat4& view, const glm::mat4& projection);
	void CleanUpAll();

private:
	std::string m_name;
	std::vector<std::unique_ptr<Mesh3D>> m_objects;
	GLuint m_shaderProgram;
};


#endif
