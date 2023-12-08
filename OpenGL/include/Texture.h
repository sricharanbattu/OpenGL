#pragma once


#include "import_gl.h"
#include <string>

//void AttachATextureToFBOAttachment(unsigned int textureId, unsigned int fboId, );


class Texture2D{
public:
	void* m_texData = NULL;
	unsigned int m_texture_id;
	int m_width = 0;
	int m_height = 0;
	int m_nChannels = 0;
	Texture2D(void* texData, int width, int height, int nChannels);
	void BindAndAssignTexture2DData();
	void BindToTexture2D();

};

class TextureData {
public:
	unsigned char* m_texData = NULL;
	int m_width;
	int m_height;
	int m_nChannels;
	TextureData(int width, int height, int nChannels);
	~TextureData();
};

