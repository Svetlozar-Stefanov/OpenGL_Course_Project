#include "Light.h"

Light::Light()
{
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientStrength = 1.0f;

	this->diffuseStrength = 0;
}

Light::Light(const GLuint shadowWidth, const GLuint shadowHeight,
	const GLfloat red, const GLfloat green, const GLfloat blue,
	const GLfloat ambientStrength, const GLfloat diffuseStrength)
{
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);

	lightColor = glm::vec3(red, green, blue);
	this->ambientStrength = ambientStrength;

	this->diffuseStrength = diffuseStrength;
}

ShadowMap* Light::GetShadowMap() const
{
	return shadowMap;
}

Light::~Light()
{
}
