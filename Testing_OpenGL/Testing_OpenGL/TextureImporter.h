#ifndef TEXTUREIMPORTER_H
#define TEXTUREIMPORTER_H

#include <glad/glad.h>
#include <iostream>
#include <string>

#include "stb_image.h"

class TextureImporter {
public:
	unsigned int ID;
	
	TextureImporter(const std::string& texturePath, bool flipTextureVertically);
	void use(unsigned int activeTextureSlot);
};

TextureImporter::TextureImporter(const std::string& texturePath, bool flipTextureVertically) {
	int width, height, nrChannels;
	unsigned char* data;

	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_set_flip_vertically_on_load(flipTextureVertically);
	data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		if (texturePath.find(".png") != std::string::npos) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (texturePath.find(".jpg") != std::string::npos) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
}

void TextureImporter::use(unsigned int activeTextureSlot) {
	glActiveTexture(GL_TEXTURE0 + activeTextureSlot);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

#endif