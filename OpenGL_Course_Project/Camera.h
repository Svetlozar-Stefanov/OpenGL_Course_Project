#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma once
class Camera
{
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat roll;

	GLfloat movementSpeed;
	GLfloat turnSpeed;
	GLfloat deltaTime;

	void update();
	

public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 worldUp,
		const GLfloat movementSpeed, const GLfloat turnSpeed,
		const GLfloat yaw, const GLfloat pitch);

	
	void PassKeysMovement(const bool *keys, const GLfloat deltaTime);
	void PassMouseTurn(const GLfloat changeX, const GLfloat changeY);

	glm::mat4 GetViewMatrix();

	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraDirection();
};

