#pragma once
#include "Light.h"

class DirectionalLight
	: public Light
{
private:
	glm::vec3 direction;

public:
	DirectionalLight();
	DirectionalLight(const GLuint shadowWidth, const GLuint shadowHeight, 
		const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat ambientStrength,
		const GLfloat xDir, const GLfloat yDir, const GLfloat zDir, const GLfloat diffuseStrength);
	~DirectionalLight();

	glm::mat4 CalculateLightTransform();

	void UseLight(const GLint ambientColorLocation, const GLint ambientStrengthLocation,
		const GLint diffuseDirLocation, const GLint diffuseStrengthLocation);

};

