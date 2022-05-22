#include "SpotLight.h"

SpotLight::SpotLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
}

SpotLight::SpotLight(const GLfloat shadowWidth, const GLfloat shadowHeight,
	const GLfloat near, const GLfloat far,
	const GLfloat red, const GLfloat green, const GLfloat blue,
	const GLfloat ambientStrength, const GLfloat diffuseStrength,
	const GLfloat xPos, const GLfloat yPos, const GLfloat zPos, const GLfloat xDir, const GLfloat yDir, const GLfloat zDir,
	const GLfloat constant, const GLfloat linear, const GLfloat exponent, const GLfloat edge)
	: PointLight(shadowWidth, shadowHeight,red, green, blue,near, far, ambientStrength, diffuseStrength, xPos, yPos, zPos, constant, linear, exponent)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	this->edge = edge;
	processedEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(const GLint ambientColorLocation, const GLint ambientStrengthLocation, const GLint diffuseStrengthLocation, const GLint positionLocation, const GLint directionLocation, const GLint constantLocation, const GLint linearLocation, const GLint exponentLocation, const GLint edgeLocation)
{
	glUniform3f(ambientColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glUniform1f(ambientStrengthLocation, ambientStrength);
	glUniform1f(diffuseStrengthLocation, diffuseStrength);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}
