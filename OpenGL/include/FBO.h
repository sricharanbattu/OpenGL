#pragma once
#include "import_gl.h"
class Framebuffer {
public:
	unsigned int m_fboId;
	int m_width;
	int m_height;
	int m_nChannels=3;
	unsigned int m_colorAttachment0_Id;
	Framebuffer(int width, int height, int nChannels);
	~Framebuffer();
	void BindFramebuffer();
	void UnbindFramebuffer();
	void DisplayFramebufferOnScreen();
};
