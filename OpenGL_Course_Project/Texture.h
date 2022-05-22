#pragma once
#include <GL/glew.h>

#include "CommonValues.h"

class Texture
{
private:
	const int MAX_FILE_NAME_LEN = 100;

	GLuint textureId;
	int width, height, bitDepth;

	char* fileLocation;
public:
	Texture();
	Texture(const char* location);

	bool LoadTextureRGBA();
	bool LoadTextureRGB();

	void UseTexture();
	void ClearTexture();

	~Texture();

};

