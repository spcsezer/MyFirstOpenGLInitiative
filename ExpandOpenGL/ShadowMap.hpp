#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{

protected:

	GLuint FBO, shadowMap;
	GLuint shadowWidth, shadowHeight;

public:

	ShadowMap();

	virtual bool Init(GLuint width, GLuint height);

	virtual void Write();

	virtual void Read(GLenum textureUnit);

	GLuint getShadowWitdth() { return shadowWidth; }
	GLuint getShadowHeight() { return shadowHeight; }

	~ShadowMap();

};

