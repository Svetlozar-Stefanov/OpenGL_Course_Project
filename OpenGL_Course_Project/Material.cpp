#include "Material.h"

Material::Material()
{
	reflectivness = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat reflectivness, GLfloat shininess)
{
	this->reflectivness = reflectivness;
	this->shininess = shininess;
}

void Material::UseMaterial(const GLint reflectivenessLocation, const GLint shininessLocation)
{
	glUniform1f(reflectivenessLocation, reflectivness);
	glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{
}
