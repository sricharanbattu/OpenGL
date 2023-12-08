#include "Texture.h"


Texture2D::Texture2D(void* texData, int width, int height, int nChannels) : m_texData{texData}, m_width{width}, m_height{height}, m_nChannels{nChannels}
{
	glGenTextures(1, &m_texture_id);
}

void Texture2D::BindAndAssignTexture2DData()
{
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	if (m_texData != NULL)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_texData);
	}
	else
	{
		std::cout << "ERROR :: Texture not correct\n";
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::BindToTexture2D()
{
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


TextureData::TextureData(int width, int height, int nChannels) : m_width{ width }, m_height{ height }, m_nChannels{ nChannels }
{
	m_texData = new unsigned char[width * height * nChannels];
}

TextureData::~TextureData()
{
	delete[] m_texData;
}
