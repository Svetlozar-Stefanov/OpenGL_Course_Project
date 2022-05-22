#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);

	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(const GLuint shadowWidth, const GLuint shadowHeight,
	const GLfloat red, const GLfloat green, const GLfloat blue, const GLfloat ambientStrength,
	const GLfloat xDir, const GLfloat yDir, const GLfloat zDir, const GLfloat diffuseStrength) : Light(shadowWidth, shadowHeight, red, green, blue, ambientStrength, diffuseStrength)
{
	direction = glm::vec3(xDir, yDir, zDir);

	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

DirectionalLight::~DirectionalLight()
{

}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void DirectionalLight::UseLight(const GLint ambientColorLocation, const GLint ambientStrengthLocation, const GLint diffuseDirLocation, const GLint diffuseStrengthLocation)
{
	glUniform3f(ambientColorLocation, lightColor.x, lightColor.y, lightColor.z);
	glUniform1f(ambientStrengthLocation, ambientStrength);

	glUniform3f(diffuseDirLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseStrengthLocation, diffuseStrength);
}
