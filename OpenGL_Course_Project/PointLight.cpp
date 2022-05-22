#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(const GLfloat shadowWidth, const GLfloat shadowHeight,
	const GLfloat red, const GLfloat green, const GLfloat blue,
	const GLfloat near, const GLfloat far,
	const GLfloat ambientStrength, const GLfloat diffuseStrength,
	const GLfloat xPos, const GLfloat yPos, const GLfloat zPos,
	const GLfloat constant, const GLfloat linear, const GLfloat exponent) : Light(shadowWidth, shadowHeight, red, green, blue, ambientStrength, diffuseStrength)
{
	position = glm::vec3(xPos, yPos, zPos);
	this->constant = constant;
	this->linear = linear;
	this->exponent = exponent;

	float aspect = (float)shadowWidth / (float)shadowHeight;

	farPlane = far;
	lightProj = glm::perspective(glm::radians(90.0f), aspect, near, farPlane);

	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

PointLight::~PointLight()
{
}

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightTransforms;
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	lightTransforms.push_back(lightProj *
		glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return lightTransforms;
}

glm::vec3 PointLight::GetPosition()
{
	return position;
}

GLfloat PointLight::GetFarPlane()
{
	return farPlane;
}

void PointLight::UseLight(const GLint ambientColorLocation, const GLint ambientStrengthLocation, const GLint diffuseStrengthLocation, 
	const GLint positionLocation, const GLint constantLocation, const GLint linearLocation, const GLint exponentLocation)
{
	glUniform3f(ambientColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glUniform1f(ambientStrengthLocation, ambientStrength);
	glUniform1f(diffuseStrengthLocation, diffuseStrength);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}