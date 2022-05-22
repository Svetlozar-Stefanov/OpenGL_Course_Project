#pragma once
#include "Light.h"
#include "OmniShadowMap.h"

#include <vector>

class PointLight :
    public Light
{
protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent;
    GLfloat farPlane;

public:
    PointLight(); 
    PointLight(const GLfloat shadowWidth, const GLfloat shadowHeight,
        const GLfloat red, const GLfloat green, const GLfloat blue,
        const GLfloat near, const GLfloat far,
        const GLfloat ambientStrength, const GLfloat diffuseStrength,
        const GLfloat xPos, const GLfloat yPos, const GLfloat zPos,
        const GLfloat constant, const GLfloat linear, const GLfloat exponent);
    ~PointLight();

    std::vector<glm::mat4> CalculateLightTransform();

    glm::vec3 GetPosition();
    GLfloat GetFarPlane();

    void UseLight(const GLint ambientColorLocation, const GLint ambientStrengthLocation, const GLint diffuseStrengthLocation, const GLint positionLocation,
        const GLint constantLocation, const GLint linearLocation , const GLint exponentLocation);
};

