#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "ShadowMap.h"

class Light
{
protected:
	glm::vec3 lightColor;
	GLfloat ambientStrength;

	GLfloat diffuseStrength;

	glm::mat4 lightProj;
	ShadowMap* shadowMap;

public:
	Light();
	Light(const GLuint shadowWidth, const GLuint shadowHeight,
		const GLfloat red, const GLfloat green, const GLfloat blue, 
		const GLfloat ambientStrength, const GLfloat diffuseStrength);
	
	ShadowMap* GetShadowMap() const;

	~Light();
};

