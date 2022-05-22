#include "Camera.h"

void Camera::update()
{
	direction.x = glm::cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	direction.y = glm::sin(glm::radians(pitch));
	direction.z = glm::cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	direction = glm::normalize(direction);

	right = glm::normalize(glm::cross(direction, worldUp));
	up = glm::normalize(glm::cross(direction, right));
}

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 worldUp, const GLfloat movementSpeed, const GLfloat turnSpeed, const GLfloat yaw, const GLfloat pitch)
{
	position = startPosition;
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	this->worldUp = worldUp;
	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;
	this->yaw = yaw;
	this->pitch = pitch;

	update();
}

void Camera::PassKeysMovement(const bool* keys, const GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += direction * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= direction * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
}

void Camera::PassMouseTurn(const GLfloat changeX, const GLfloat changeY)
{
	pitch += changeY * turnSpeed;
	yaw += changeX * turnSpeed;
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < - 89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + direction, worldUp);
}

glm::vec3 Camera::GetCameraPosition()
{
	return position;
}

glm::vec3 Camera::GetCameraDirection()
{
	return glm::normalize(direction);
}
