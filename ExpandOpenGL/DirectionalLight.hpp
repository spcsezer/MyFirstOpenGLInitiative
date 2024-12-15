#pragma once
#include "Light.hpp"

class DirectionalLight : public Light
{

	glm::vec3 direction;

public:

	DirectionalLight();
	DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
					 GLfloat red, GLfloat green, GLfloat blue, 
					 GLfloat aIntensity, GLfloat dIntensity,
					 GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

};

