#pragma once
#include "Light.hpp"

#include <vector>

#include "OmniShadowMap.hpp"

class PointLight : public Light
{

protected:

	glm::vec3 position;

	GLfloat constant, linear, exponent;

	GLfloat farPlane;

public:

	PointLight();
	PointLight(GLfloat shadowWidth, GLfloat shadowHeight,
			   GLfloat near, GLfloat far,
			   GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
			   GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
			      GLuint diffuseIntensityLocation, GLuint positionLocation,
				  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();

	GLfloat getFarPlane();
	glm::vec3 getPosition();

	~PointLight();

};

