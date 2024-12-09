#include "Texture.hpp"

Texture::Texture() {
	m_width = 0;
	m_height = 0;
	m_channels = 0;
}

bool Texture::LoadTexture(const std::string& filepath) {
    // Load image data
    unsigned char* data = stbi_load(filepath.c_str(), &m_width, &m_height, &m_channels, 0);

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Determine color format
    GLenum colorFormat;
    if (m_channels == 1)
        colorFormat = GL_RED;
    else if (m_channels == 3)
        colorFormat = GL_RGB;
    else if (m_channels == 4)
        colorFormat = GL_RGBA;
    else {
        std::cerr << "Unsupported image format" << std::endl;
        stbi_image_free(data);
        return 0;
    }

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, m_width, m_height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    Unbind();
    return 1;
}

void Texture::Bind(GLuint textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::CleanUp() {
	if (m_textureID != 0) {
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}
}