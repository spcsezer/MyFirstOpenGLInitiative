#include "Camera.hpp"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMovementSpeed, GLfloat startTurnSpeed) {

	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMovementSpeed;
	turnSpeed = startTurnSpeed;

	update();

}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {

	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {

		position += front * velocity;

	}

	if (keys[GLFW_KEY_S]) {

		position -= front * velocity;

	}

	if (keys[GLFW_KEY_A]) {

		position -= right * velocity;

	}

	if (keys[GLFW_KEY_D]) {

		position += right * velocity;

	}

}

void Camera::mouseControl(GLfloat changeX, GLfloat changeY) {

	changeX *= turnSpeed;
	changeY *= turnSpeed;

	yaw += changeX;
	pitch += changeY;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	update();
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

glm::mat4 Camera::calculateViewMatrix() {

	return glm::lookAt(position, position + front, up); //third person games you change second

}

void Camera::update() {

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front); //remove magnetude. You just want to know direction

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}

Camera::~Camera() {



}
