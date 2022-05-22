#pragma once
#include <stdio.h>
#include <string>
#include <vector>

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "CommonValues.h"

class Shader
{
private:
	GLint shaderID, uniformModel, uniformProjection, uniformView,
		uniformReflectivness, uniformShininess, uniformEyePos,
		uniformTexture, uniformDirectionalLightTransform, uniformDirectionalShadowMap,
		uniformOmniLightPos, uniformFarPlane;

	GLuint uniformLightMatrices[CUBE_MAP_SIDES];

	struct
	{
		GLint uniformAmbientStrength;
		GLint uniformAmbientColor;
		GLint uniformDirection;
		GLint uniformDiffuseStrength;

	} dLight;

	GLint uniformPointLightCount;

	struct
	{
		GLint uniformAmbientStrength;
		GLint uniformAmbientColor;
		GLint uniformDiffuseStrength;

		GLint uniformPosition;
		GLint uniformConstant;
		GLint uniformLinear;
		GLint uniformExponent;

	} pLights[MAX_POINT_LIGHTS];

	GLint uniformSpotLightCount;

	struct
	{
		GLint uniformAmbientStrength;
		GLint uniformAmbientColor;
		GLint uniformDiffuseStrength;

		GLint uniformPosition;
		GLint uniformConstant;
		GLint uniformLinear;
		GLint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;

	} sLights[MAX_SPOT_LIGHTS];

	struct
	{
		GLuint uniformShadowMap;
		GLuint uniformFarPlane;
	} uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	void AddShader(const GLuint program, const char* shaderCode, GLenum type);
	void CompileShaders(const char* vShaderCode, const char* fShaderCode);
	void CompileShaders(const char* vShaderCode, const char* gShaderCode, const char* fShaderCode);
	
	void CompileProgram();
	std::string readFile(const char* path);

public:
	Shader();
	void createFromString(const char* vShaderCode, const char* fShaderCode);
	void createFromString(const char* vShaderCode, const char* gShaderCode, const char* fShaderCode);

	void createFromFile(const char* vShaderPath, const char* fShaderPath);
	void createFromFile(const char* vShaderPath, const char* gShaderPath, const char* fShaderPath);

	void Validate();

	void use();

	void SetDirectionalLight(DirectionalLight* dirLight);
	void SetPointLights(PointLight* pointLights, unsigned int pointLightsCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLights(SpotLight* spotLights, unsigned int spotLightsCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4 transform);

	void SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices);

	void clear();

	~Shader();

	 GLint getUniformModel();
	 GLint getUniformProjection();
	 GLint getUniformView();
	 GLint getUniformAmbientStrength();
	 GLint getUniformAmbientColor();
	 GLint getUniformDirection();
	 GLint getUniformDiffuseStrength();
	 GLint getUniformReflectivness();
	 GLint getUniformShininess();
	 GLint getUniformEyePos();
	 GLuint GetOmniLightPos();
	 GLuint GetFarPlane();
};

