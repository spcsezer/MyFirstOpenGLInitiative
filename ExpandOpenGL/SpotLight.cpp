#include "SpotLight.hpp"

SpotLight::SpotLight() : PointLight()
{

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));

}

SpotLight::SpotLight(GLfloat shadowWidth, GLfloat shadowHeight,
					GLfloat near, GLfloat far,
					GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity, GLfloat dIntensity, 
					GLfloat xPos, GLfloat yPos, GLfloat zPos, 
					GLfloat xDir, GLfloat yDir, GLfloat zDir, 
					GLfloat con, GLfloat lin, GLfloat exp, 
					GLfloat edg) : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{

	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));

	edge = edg;
	procEdge = cosf(glm::radians(edge));

}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
						GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation, 
						GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, 
						GLuint edgeLocation)
{

	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);

}

SpotLight::~SpotLight()
{



}
