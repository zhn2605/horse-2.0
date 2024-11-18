#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

Camera::Camera() {
    eye = glm::vec3(0.0f, 0.0f, 0.0f);
    lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::GetViewMatrix() const{
 	return glm::lookAt(eye, eye + lookDirection, upVector);
}

void Camera::MouseLook(int mouseX, int mouseY) {
    std::cout << "mouse: " << mouseX << ", " << mouseY << std::endl;

    glm::vec2 currentMouse = glm::vec2((float)mouseX, (float)mouseY);

    static bool firstLook = true;
    if (firstLook) {
        oldMousePos = currentMouse;
        firstLook = false;
    }

    glm::vec2 mouseDelta = oldMousePos - currentMouse;

    // Horizontal Look
    lookDirection = glm::rotate(lookDirection, .1f * glm::radians(mouseDelta.x), upVector);

    // Vertical Look
    glm::vec3 rightVector = glm::normalize(glm::cross(lookDirection, upVector));
    lookDirection = glm::rotate(lookDirection, .1f * glm::radians(mouseDelta.y), rightVector);

    lookDirection = glm::normalize(lookDirection);

    oldMousePos = currentMouse;
}

void Camera::MoveForward(float speed) {
    eye += (lookDirection * speed);
}

void Camera::MoveBackward(float speed) {
    eye -= (lookDirection * speed);
}

void Camera::MoveLeft(float speed) {
    glm::vec3 rightVector = glm::normalize(glm::cross(lookDirection, upVector));
    eye -= (rightVector * speed);
}

void Camera::MoveRight(float speed) {
    glm::vec3 rightVector = glm::normalize(glm::cross(lookDirection, upVector));
    eye += (rightVector * speed);
}

void Camera::MoveUp(float speed) {
    eye.y += glm::normalize(upVector).y * speed;
}

void Camera::MoveDown(float speed) {
    eye.y -= glm::normalize(upVector).y * speed;
}