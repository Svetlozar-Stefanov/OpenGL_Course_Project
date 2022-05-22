#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
private:
    glm::vec3 direction;

    GLfloat edge, processedEdge;

public:
    SpotLight();

    SpotLight(
        const GLfloat shadowWidth, const GLfloat shadowHeight,
        const GLfloat near, const GLfloat far,
        const GLfloat red, const GLfloat green, const GLfloat blue,
        const GLfloat ambientStrength, const GLfloat diffuseStrength,
        const GLfloat xPos, const GLfloat yPos, const GLfloat zPos,
        const GLfloat xDir, const GLfloat yDir, const GLfloat zDir,
        const GLfloat constant, const GLfloat linear, const GLfloat exponent, const GLfloat edge);


    void UseLight(const GLint ambientColorLocation, const GLint ambientStrengthLocation, const GLint diffuseStrengthLocation, 
        const GLint positionLocation, const GLint directionLocation, 
        const GLint constantLocation, const GLint linearLocation, const GLint exponentLocation, const GLint edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir);

    ~SpotLight();
};

