#pragma once
#include "import_gl.h"
#include "Texture.h"
#include "FBO.h"

void DisplayDataOnScreen(void* textureData, int width, int height, int nChannels);
void DisplayTextureOnScreen(unsigned int* pTextureId, GLFWwindow* window);
void DisplayAnFboOnScreen(unsigned int fbo_id, int width, int height, int nChannels);
//void BindAndAssignTexture2DData(Texture2D texInfo);
//void BindToTexture2D(Texture2D tex);
void RenderTexture2DDataOntoFBO(Framebuffer& fbo, Texture2D& tex);
unsigned int GenericDataSetupForRendering();
