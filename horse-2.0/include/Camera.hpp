#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
	
	Camera();

	glm::mat4 GetViewMatrix() const;

	void MouseLook(int mouseX, int mouseY);
	void MoveForward(float speed);
	void MoveBackward(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

private:
	glm::vec3 eye;
	glm::vec3 lookDirection;
	glm::vec3 upVector;

	glm::vec2 oldMousePos;	// store old mouse pos
};

#endif