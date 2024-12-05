#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Texture {
public:
	Texture();
	
	void LoadTexture(const std::string& filepath);
	void Bind(GLuint textureUnit = 0);
	void Unbind();
	void CleanUp();

private:
	GLuint m_textureID = 0;
	int m_width, m_height, m_channels;
	std::string m_filepath;
};


#endif
