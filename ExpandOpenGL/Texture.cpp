#include "Texture.hpp"


Texture::Texture() {

	textureID = width = height = bitDepth = 0;
	fileLocation = "";

}

Texture::Texture(const char* fileLoc) {

	textureID = width = height = bitDepth = 0;
	fileLocation = fileLoc;

}

bool Texture::LoadTexture() {

	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData) {
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//we can change for customize the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

bool Texture::LoadTextureA() {

	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
	if (!texData) {
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//we can change for customize the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

void Texture::UseTexture() {

	//You can activate more than 1 textures at the same time (check for this, tutorial)
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::ClearTexture() {

	glDeleteTextures(1, &textureID);
	textureID = width = height = bitDepth = 0;
	fileLocation = "";

}


Texture::~Texture() {
	ClearTexture();
}