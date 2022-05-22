#include "Shader.h"
#include <fstream>

void Shader::AddShader(const GLuint program, const char* shaderCode, GLenum type)
{
	GLint shader = glCreateShader(type);
	const GLchar* code[1];
	code[0] = shaderCode;

	GLint length[1];
	length[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, length);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	GLint size = 0;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, 1024, &size, eLog);
		printf("Error compiling the %d shader: %s\n", type, eLog);
		return;
	}

	glAttachShader(program, shader);
}

void Shader::CompileShaders(const char* vShaderCode, const char* fShaderCode)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		printf("Failed to create shader program!\n");
		return;
	}

	AddShader(shaderID, vShaderCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fShaderCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileShaders(const char* vShaderCode, const char* gShaderCode, const char* fShaderCode)
{
	shaderID = glCreateProgram();
	if (!shaderID)
	{
		printf("Failed to create shader program!\n");
		return;
	}

	AddShader(shaderID, vShaderCode, GL_VERTEX_SHADER);
	AddShader(shaderID, gShaderCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fShaderCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

void Shader::CompileProgram()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	dLight.uniformAmbientColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	dLight.uniformAmbientStrength = glGetUniformLocation(shaderID, "directionalLight.base.ambientStrength");
	dLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	dLight.uniformDiffuseStrength = glGetUniformLocation(shaderID, "directionalLight.base.diffuseStrength");
	uniformReflectivness = glGetUniformLocation(shaderID, "material.reflectivness");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePos = glGetUniformLocation(shaderID, "eyePos");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (int i = 0; i < CUBE_MAP_SIDES; i++)
	{
		char buff[100] = { "\0" };

		snprintf(buff, sizeof(buff), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, buff);
	}

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char buff[100] = { "\0" };

		snprintf(buff, sizeof(buff), "pLights[%d].base.color", i);
		pLights[i].uniformAmbientColor = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "pLights[%d].base.ambientStrength", i);
		pLights[i].uniformAmbientStrength = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "pLights[%d].base.diffuseStrength", i);
		pLights[i].uniformDiffuseStrength = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "pLights[%d].position", i);
		pLights[i].uniformPosition = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "pLights[%d].constant", i);
		pLights[i].uniformConstant = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "pLights[%d].linear", i);
		pLights[i].uniformLinear = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "pLights[%d].exponent", i);
		pLights[i].uniformExponent = glGetUniformLocation(shaderID, buff);

	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char buff[100] = { "\0" };

		snprintf(buff, sizeof(buff), "sLights[%d].point.base.color", i);
		sLights[i].uniformAmbientColor = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].point.base.ambientStrength", i);
		sLights[i].uniformAmbientStrength = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].point.base.diffuseStrength", i);
		sLights[i].uniformDiffuseStrength = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].point.position", i);
		sLights[i].uniformPosition = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].point.constant", i);
		sLights[i].uniformConstant = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].point.linear", i);
		sLights[i].uniformLinear = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].point.exponent", i);
		sLights[i].uniformExponent = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].direction", i);
		sLights[i].uniformDirection = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "sLights[%d].edge", i);
		sLights[i].uniformEdge = glGetUniformLocation(shaderID, buff);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	for (int i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char buff[100] = { "\0" };

		snprintf(buff, sizeof(buff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMaps[i].uniformShadowMap = glGetUniformLocation(shaderID, buff);

		snprintf(buff, sizeof(buff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMaps[i].uniformFarPlane = glGetUniformLocation(shaderID, buff);
	}
}

std::string Shader::readFile(const char* path)
{
	std::fstream file;
	std::string output;
	file.open(path, std::fstream::in);
	if (!file.is_open())
	{
		printf("Unable to open file");
		return output;
	}

	std::string buffer = "";
	while (!file.eof())
	{
		std::getline(file, buffer);
		output.append(buffer);
		output.append("\n");
	}
	output.append("\0");

	file.close();

	return output;
}

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
}

void Shader::createFromString(const char* vShaderCode, const char* fShaderCode)
{
	CompileShaders(vShaderCode, fShaderCode);
}

void Shader::createFromString(const char* vShaderCode, const char* gShaderCode, const char* fShaderCode)
{
	CompileShaders(vShaderCode, gShaderCode,fShaderCode);
}

void Shader::createFromFile(const char* vShaderPath, const char* fShaderPath)
{
	std::string vCode = readFile(vShaderPath);
	std::string fCode = readFile(fShaderPath);

	const char* vShaderCode = vCode.c_str();
	const char* fShaderCode = fCode.c_str();

	createFromString(vShaderCode, fShaderCode);
}

void Shader::createFromFile(const char* vShaderPath, const char* gShaderPath, const char* fShaderPath)
{
	std::string vCode = readFile(vShaderPath);
	std::string gCode = readFile(gShaderPath);
	std::string fCode = readFile(fShaderPath);

	const char* vShaderCode = vCode.c_str();
	const char*	gShaderCode = gCode.c_str();
	const char* fShaderCode = fCode.c_str();

	createFromString(vShaderCode, gShaderCode ,fShaderCode);
}

void Shader::use()
{
	if (shaderID == 0)
	{
		return;
	}
	glUseProgram(shaderID);
}

void Shader::SetDirectionalLight(DirectionalLight* dirLight)
{
	dirLight->UseLight(dLight.uniformAmbientColor, dLight.uniformAmbientStrength, dLight.uniformDirection, dLight.uniformDiffuseStrength);
}

void Shader::SetPointLights(PointLight* pointLights, unsigned int pointLightsCount, unsigned int textureUnit, unsigned int offset)
{
	if (pointLightsCount > MAX_POINT_LIGHTS)
	{
		pointLightsCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, pointLightsCount);

	for (int i = 0; i < pointLightsCount; i++)
	{
		pointLights[i].UseLight(pLights[i].uniformAmbientColor, pLights[i].uniformAmbientStrength, pLights[i].uniformDiffuseStrength, 
			pLights[i].uniformPosition, pLights[i].uniformConstant, pLights[i].uniformLinear, pLights[i].uniformExponent);

		pointLights[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + offset].uniformShadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].uniformFarPlane, pointLights[i].GetFarPlane());
	}

}

void Shader::SetSpotLights(SpotLight* spotLights, unsigned int spotLightsCount, unsigned int textureUnit, unsigned int offset)
{
	if (spotLightsCount > MAX_SPOT_LIGHTS)
	{
		spotLightsCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, spotLightsCount);

	for (int i = 0; i < spotLightsCount; i++)
	{
		spotLights[i].UseLight(sLights[i].uniformAmbientColor, sLights[i].uniformAmbientStrength, sLights[i].uniformDiffuseStrength,
			sLights[i].uniformPosition, sLights[i].uniformDirection, 
			sLights[i].uniformConstant, sLights[i].uniformLinear, sLights[i].uniformExponent, sLights[i].uniformEdge);

		spotLights[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + offset].uniformShadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].uniformFarPlane, spotLights[i].GetFarPlane());
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 transform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (int i = 0; i < CUBE_MAP_SIDES; i++)
	{
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

void Shader::clear()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	
	uniformModel = 0;
	uniformProjection = 0;
}

Shader::~Shader()
{
	clear();
}

GLint Shader::getUniformModel()
{
	return	uniformModel;
}

GLint Shader::getUniformProjection()
{
	return uniformProjection;
}

GLint Shader::getUniformView()
{
	return uniformView;
}

GLint Shader::getUniformAmbientStrength()
{
	return dLight.uniformAmbientStrength;
}

GLint Shader::getUniformAmbientColor()
{
	return dLight.uniformAmbientColor;
}

GLint Shader::getUniformDirection()
{
	return dLight.uniformDirection;
}

GLint Shader::getUniformDiffuseStrength()
{
	return dLight.uniformDiffuseStrength;
}

GLint Shader::getUniformReflectivness()
{
	return uniformReflectivness;
}

GLint Shader::getUniformShininess()
{
	return uniformShininess;
}

GLint Shader::getUniformEyePos()
{
	return uniformEyePos;
}

GLuint Shader::GetOmniLightPos()
{
	return uniformOmniLightPos;
}

GLuint Shader::GetFarPlane()
{
	return uniformFarPlane;
}
