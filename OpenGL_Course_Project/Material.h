#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Material
{
private:
	GLfloat reflectivness;
	GLfloat shininess;

public:
	Material();
	Material(GLfloat reflectivness, GLfloat shininess);

	void UseMaterial(const GLint reflectivenessLocation, const GLint shininessLocation);

	~Material();

};

