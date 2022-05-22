#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Mesh.h"
#include "Shader.h"
#include "glWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "CommonValues.h"

#include "Model.h"
#include "Skybox.h"

const GLint WIDTH = 1280;
const GLint HEIGHT = 720;
const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformReflectivness = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0,
uniformOmniLightPos = 0, uniformFarPlane = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

std::vector<Shader*> shaders;
Shader* directionalShadowShader;
Shader* omniShadowShader;

std::vector<Mesh*> meshes;

Camera camera;

DirectionalLight sunlight;
PointLight pointLights[MAX_POINT_LIGHTS];
int pointLightCount = 0;

SpotLight spotLights[MAX_SPOT_LIGHTS];
int spotLightCount = 0;

Skybox skybox;

Texture* brickTexture;
Texture* dirtTexture;
Texture* plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model xWing;

void setDeltaTime() 
{
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
}

void CalculateNormals(const unsigned int *indices, const unsigned int indicesCount, GLfloat * vertices, const unsigned int verticesCount, 
	const unsigned int vLength, const unsigned int normalOffset) 
{
	for (rsize_t i = 0; i < indicesCount; i += 3)
	{
		unsigned int dI0 = indices[i] * vLength;
		unsigned int dI1 = indices[i + 1] * vLength;
		unsigned int dI2 = indices[i + 2] * vLength;

		glm::vec3 vec1 = glm::vec3(vertices[dI1] - vertices[dI0], vertices[dI1 + 1] - vertices[dI0 + 1], vertices[dI1 + 2] - vertices[dI0 + 2]);
		glm::vec3 vec2 = glm::vec3(vertices[dI2] - vertices[dI0], vertices[dI2 + 1] - vertices[dI0 + 1], vertices[dI2 + 2] - vertices[dI0 + 2]);

		glm::vec3 normal = glm::normalize(glm::cross(vec1, vec2));

		vertices[dI0 + normalOffset] += normal.x;
		vertices[dI0 + normalOffset + 1] += normal.y;
		vertices[dI0 + normalOffset + 2] += normal.z;

		vertices[dI1 + normalOffset] += normal.x;
		vertices[dI1 + normalOffset + 1] += normal.y;
		vertices[dI1 + normalOffset + 2] += normal.z;

		vertices[dI2 + normalOffset] += normal.x;
		vertices[dI2 + normalOffset + 1] += normal.y;
		vertices[dI2 + normalOffset + 2] += normal.z;
	}

	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned int nI = (i * vLength) + normalOffset;

		GLfloat nX = vertices[nI];
		GLfloat nY = vertices[nI + 1];
		GLfloat nZ = vertices[nI + 2];

		glm::vec3 nVec = glm::vec3(nX, nY, nZ);
		nVec = glm::normalize(nVec);

		vertices[nI] = nVec.x;
		vertices[nI + 1] = nVec.y;
		vertices[nI + 2] = nVec.z;
	}
}

void CreateObjects() 
{
	unsigned int indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	CalculateNormals(indices, 12, vertices, 32, 8, 5);

	unsigned int planeIndices[] =
	{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat planeVertices[] =
	{
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,	0.0f,	-1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,		0.0f, -1.0f, 0.0f
	};

	Mesh* obj = new Mesh;
	obj->CreateMesh(vertices, indices, 32, 12);
	meshes.push_back(obj);

	Mesh* plane = new Mesh;
	plane->CreateMesh(planeVertices, planeIndices, 32, 6);
	meshes.push_back(plane);
}

void RenderScene() 
{
	////obj1
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(-5.0f, -1.0f, 0.0f));
	model = glm::rotate(model, 45 * toRadians ,glm::vec3(0.0f, 1.0f, 0.0f));
	
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	brickTexture->UseTexture();
	//shinyMaterial.UseMaterial(shaders[0]->getUniformReflectivness(), shaders[0]->getUniformShininess());
	dullMaterial.UseMaterial(uniformReflectivness, uniformShininess);

	meshes[0]->RenderMesh();

	//obj2
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
	model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	dirtTexture->UseTexture();
	//shinyMaterial.UseMaterial(uniformReflectivness, uniformShininess);
	dullMaterial.UseMaterial(shaders[0]->getUniformReflectivness(), shaders[0]->getUniformShininess());

	meshes[1]->RenderMesh();

	//obj3
	model = glm::mat4(1.0f);

	model = glm::translate(model, glm::vec3(0.0f, -2.0f, -1.0f));
	model = glm::rotate(model, 90 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));


	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	shinyMaterial.UseMaterial(uniformReflectivness, uniformShininess);
	//dullMaterial.UseMaterial(shaders[0]->getUniformReflectivness(), shaders[0]->getUniformShininess());

	xWing.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* directionalLight) 
{
	directionalShadowShader->use();

	glViewport(0,0,directionalLight->GetShadowMap()->GetShadowWidth(), directionalLight->GetShadowMap()->GetShadowHeight());

	directionalLight->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader->getUniformModel();
 	directionalShadowShader->SetDirectionalLightTransform(directionalLight->CalculateLightTransform());

	directionalShadowShader->Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowPass(PointLight* light)
{
	omniShadowShader->use();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader->getUniformModel();
	uniformOmniLightPos = omniShadowShader->GetOmniLightPos();
	uniformFarPlane = omniShadowShader->GetFarPlane();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

	omniShadowShader->SetOmniLightMatrices(light->CalculateLightTransform());

	omniShadowShader->Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatirx, glm::mat4 viewMatrix) 
{
	glViewport(0, 0, WIDTH, HEIGHT);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatirx);

	shaders[0]->use();

	uniformModel = shaders[0]->getUniformModel();
	uniformProjection = shaders[0]->getUniformProjection();
	uniformView = shaders[0]->getUniformView();
	uniformEyePosition = shaders[0]->getUniformEyePos();
	uniformReflectivness = shaders[0]->getUniformReflectivness();
	uniformShininess = shaders[0]->getUniformShininess();

	
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatirx));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

	shaders[0]->SetDirectionalLight(&sunlight);
	shaders[0]->SetPointLights(pointLights, pointLightCount, 3, 0); 
	shaders[0]->SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaders[0]->SetDirectionalLightTransform(sunlight.CalculateLightTransform());

	sunlight.GetShadowMap()->Read(GL_TEXTURE2);
	shaders[0]->SetTexture(1);
	shaders[0]->SetDirectionalShadowMap(2);
	
	//spotLights[0].SetFlash(camera.GetCameraPosition(), camera.GetCameraDirection());

	shaders[0]->Validate();

	RenderScene();
}

int main()
{
	glWindow mainWindow(WIDTH, HEIGHT);
	mainWindow.initialize();

	shaders.push_back(new Shader());
	shaders[0]->createFromFile("Shaders/vShader.txt", "Shaders/fShader.txt");

	directionalShadowShader = new Shader();
	directionalShadowShader->createFromFile("Shaders/vDirectional_shadow_map.txt", "Shaders/fDirectional_shadow_map.txt");

	omniShadowShader = new Shader();
	omniShadowShader->createFromFile("Shaders/vOmni_shadow_map.txt", "Shaders/gOmni_shadow_map.txt", "Shaders/fOmni_shadow_map.txt");

	CreateObjects(); 

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f, 0.5f, 0.0f, 0.0f);

	brickTexture = new Texture("Textures/brick.png");
	brickTexture->LoadTextureRGBA();

	dirtTexture = new Texture("Textures/dirt.png");
	dirtTexture->LoadTextureRGBA();

	plainTexture = new Texture("Textures/plain.png");
	plainTexture->LoadTextureRGBA();

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(1.0f, 3);

	sunlight = DirectionalLight(1024, 1024, 1.0f, 0.53f, 0.3f, 0.1f, 
		-10.0f, -12.0f, 18.5f, 0.9f);

	pointLights[pointLightCount] = PointLight(1024, 1024,0.0f, 1.0f, 0.0f, 0.01f, 100.0f,
								1.0f, 1.0f, 2.0f, 1.0f, 0.0f, 
								0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	pointLights[pointLightCount] = PointLight(1024, 1024, 0.0f, 0.0f, 1.0f, 0.01f, 100.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	spotLights[spotLightCount] = SpotLight(1024, 1024, 0.01f, 100.0f,1.0f, 1.0f, 1.0f,
		0.6f, 1.0f,
		0.0f, 2.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);

	//spotLightCount++;

	spotLights[spotLightCount] = SpotLight(1024, 1024, 0.01f, 100.0f, 1.0f, 1.0f, 1.0f,
		0.6f, 1.0f,
		2.0f, 2.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);

	//spotLightCount++;

	std::vector<std::string>skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	
	skybox = Skybox(skyboxFaces);

	xWing.LoadModel("Models/uh60.obj");

	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
		((GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight()), 0.1f, 100.0f);
	while (!mainWindow.GetShouldClose())
	{
		setDeltaTime();

		glfwPollEvents();

		camera.PassKeysMovement(mainWindow.getKeys(), deltaTime);
		camera.PassMouseTurn(mainWindow.getXChange(), mainWindow.getYChange());

		DirectionalShadowMapPass(&sunlight);
		for (int i = 0; i < pointLightCount; i++)
		{
			OmniShadowPass(&pointLights[i]);
		}
		for (int i = 0; i < spotLightCount; i++)
		{
			OmniShadowPass(&spotLights[i]);
		}
		
		RenderPass(projection, camera.GetViewMatrix());

		glUseProgram(0);

		mainWindow.swapBuffers();
	}
}
