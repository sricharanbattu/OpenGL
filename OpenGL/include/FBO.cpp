#include "FBO.h"
#include "utilities.h"

Framebuffer::Framebuffer(int width, int height, int nChannels) : m_width{width}, m_height{height}, m_nChannels{nChannels}
{
	glGenFramebuffers(1, &m_fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

	glGenTextures(1, &m_colorAttachment0_Id);
	glBindTexture(GL_TEXTURE_2D, m_colorAttachment0_Id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment0_Id, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR :: FrameBuffer not complete;\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Framebuffer::DisplayFramebufferOnScreen()
{
	DisplayAnFboOnScreen(m_fboId, m_width, m_height, m_nChannels);
	
}

void Framebuffer::BindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
}

void Framebuffer::UnbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_fboId);
}







