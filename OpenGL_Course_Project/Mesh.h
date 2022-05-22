#pragma once
#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(const GLfloat* vertices, const unsigned int* indices,
		const unsigned int numOfVertecies, const unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, EBO;
	GLsizei indexCount;
};

