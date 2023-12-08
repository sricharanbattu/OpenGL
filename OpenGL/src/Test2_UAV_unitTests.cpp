#include "../include/import_gl.h"

#include "Test2_UAV_unitTests.h"
#include "Test2_UAV_utilities.h"
#include "../include/utilities.h"

#include "../include/Shaders.h"
#include "../include/FBO.h"
//#include "../include/Texture.h"


void UnitTest1_TextureCreateTest()
{
	std::cout << "Hello, World!" << '\n';

	int width = 800;
	int height = 600;
	int nChannels = 3;
	unsigned char* texture1Data = new unsigned char[width * height * nChannels];
	//std::cout << sizeof(texture1Data)/sizeof(texture1Data[0]);
	for (int i = 0; i < (width * height); i++)
	{
		for (int j = 0; j < nChannels; j++)
		{
			if (i & 1)
			{
				texture1Data[(i * nChannels) + j] = 255;
			}
			else
			{
				texture1Data[(i * nChannels) + j] = 0;
			}


			//std::cout << unsigned(texture1Data[(i * nChannels) + j]) << '\n';
		}
		texture1Data[(i * nChannels) + nChannels - 1] = 0xFF;

	}
	unsigned char* ptr = texture1Data;
	for (int i = 0; i < height; i++)
	{
		unsigned char data = (i & 8) ? 0x00 : 0xFF;
		for (int j = 0; j < width; j++)
		{
			*ptr++ = (j & 8) ? data : ~data;
			*ptr++ = (j & 8) ? data : ~data;
			*ptr++ = (j & 8) ? data : ~data;

		}
	}

	

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	DisplayDataOnScreen(texture1Data, width, height, nChannels);
	delete[] texture1Data;

}


void UnitTest2_FBODisplayTest()
{
	unsigned char* texture1Data;
	unsigned char* texture2Data;

	int width1, width2, height1, height2, nChannels1, nChannels2;

	width1 = 800;
	width2 = 800;
	height1 = 600;
	height2 = 600;
	nChannels1 = 3;
	nChannels2 = 3;

	texture1Data = new unsigned char[width1*height1*nChannels1];
	texture2Data = new unsigned char[width2 * height2 * nChannels2];

	unsigned char* ptr1 = texture1Data;

	for (int i = 0; i < width1; i++)
	{
		for (int j = 0; j < height1; j++)
		{
			if (i < width1 / 3)
			{
				*ptr1++ = 0xFF;
				*ptr1++ = 0x00;
				*ptr1++ = 0x00;
			}
			else if (i < 2 * width1 / 3)
			{
				*ptr1++ = 0x00;
				*ptr1++ = 0xFF;
				*ptr1++ = 0x00;
			}
			else
			{
				*ptr1++ = 0x00;
				*ptr1++ = 0x00;
				*ptr1++ = 0xFF;
			}
		}
	}

	//DisplayDataOnScreen(texture1Data, width1, height1, nChannels1);
	//delete[] texture1Data;

	ptr1 = texture2Data;
	for (int i = 0; i < width2; i++)
	{
		for (int j = 0; j < height2; j++)
		{
			if (i < width2 / 4)
			{
				*ptr1++ = 0xFF;
				*ptr1++ = 0xFF;
				*ptr1++ = 0x00;
			}
			else if (i < width2/2)
			{
				*ptr1++ = 0x00;
				*ptr1++ = 0xFF;
				*ptr1++ = 0xFF;
			}
			else
			{
				*ptr1++ = 0xFF;
				*ptr1++ = 0x00;
				*ptr1++ = 0xFF;
			}
		}
	}


	// Test Proceeds as follows. 
	// Attach texture1 to FBO1 attachment 0. Render to it.
	// Attach texture1 and texture 2 to image units.
	// Read from image unit 1 to another fbo. Read to image unit 1 from image unit 2. 

	//unsigned int texId1, texId2;
	//glGenTextures(1, &texId1);
	//glGenTextures(1, &texId2);

	Texture2D texture1 = Texture2D(texture1Data, width1, height1, nChannels1);
	

	Framebuffer fbo1 = Framebuffer(width1, height1, nChannels1);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo1.m_fboId);
	texture1.BindAndAssignTexture2DData();
	std::string vert_shader_path = "res/shaders/VS_passPosAndTex.vert";
	std::string frag_shader_path = "res/shaders/FS_SampleTexture.frag";
	
	Shader shader1 = Shader(vert_shader_path, GL_VERTEX_SHADER);
	Shader shader2 = Shader(frag_shader_path, GL_FRAGMENT_SHADER);

	//shader2.PrintShaderCode();
	Program program1 = Program(shader1, shader2);
	program1.UseProgram();
	glUniform1i(glGetUniformLocation(program1.m_program_id, "ourTexture"), 0);
	
	

	//Data to render on 
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f

	};
	unsigned int indices[] = { 0,1,2,0,2,3 };

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//
	glBindVertexArray(VAO);
	glDepthMask(GL_FALSE);
	glClearColor(1.0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo1.m_fboId);
	texture1.BindToTexture2D();
	program1.UseProgram();
	glUniform1i(glGetUniformLocation(program1.m_program_id, "ourTexture"), 0);
	//
	int currentFBOId, currentTex2DId;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBOId);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTex2DId);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glFinish();
	//unsigned char* fboData = new unsigned char[width1 * height1 * nChannels1];
	//glReadPixels(0, 0, width1, height1, GL_RGB, GL_UNSIGNED_BYTE, fboData);
	//std::cout << fboData[0] << '\n';
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//DisplayTextureOnScreen(&fbo1.m_colorAttachment0_Id, glfwGetCurrentContext());
	//DisplayAnFboOnScreen(fbo1.m_fboId, width1, height1, nChannels1);
	fbo1.DisplayFramebufferOnScreen();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//delete[] fboData;
	delete[] texture1Data;
	delete[] texture2Data;
}

void UnitTest3_UAVTest()
{
	//How should this test proceed?
	// Have a texture1
	// Attach it to a image unit. 
	// Read data from texture2 and write it to texture1. 
	// Read data from texture1 and write it to texture 2. 

	int width, height, nChannels;
	width = 800;
	height = 600;
	nChannels = 3;
	TextureData texData1 = TextureData(width, height, nChannels);
	TextureData texData2 = TextureData(width, height, nChannels);
	Texture2D tex1 = Texture2D(texData1.m_texData, width, height, nChannels);
	Texture2D tex2 = Texture2D(texData2.m_texData, width, height, nChannels);
	Framebuffer fbo1(width, height, nChannels);
	Framebuffer fbo2(width, height, nChannels);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			unsigned char* ptr;
			ptr = texData1.m_texData + ((j * width) + i) * nChannels;
			if (j < height / 2)
			{
				*ptr++ = 0xFF;
				*ptr++ = 0x00;
				*ptr++ = 0x00;
			}
			else
			{
				*ptr++ = 0x00;
				*ptr++ = 0xFF;
				*ptr++ = 0x00;
			}
		}
	}

	for (int i = 0; i< width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			unsigned char* ptr;
			ptr = texData2.m_texData + ((j * width) + i) * nChannels;
			if (i < width / 2)
			{
				*ptr++ = 0xFF;
				*ptr++ = 0xFF;
				*ptr++ = 0x00;
			}
			else
			{
				*ptr++ = 0x00;
				*ptr++ = 0xFF;
				*ptr++ = 0xFF;
			}
		}
	}

	tex1.BindAndAssignTexture2DData();
	tex2.BindAndAssignTexture2DData();

	//DisplayTextureOnScreen(&tex2.m_texture_id, glfwGetCurrentContext());
	RenderTexture2DDataOntoFBO(fbo1, tex1);
	//fbo1.DisplayFramebufferOnScreen();

	fbo1.UnbindFramebuffer();
	glBindImageTexture(4, fbo1.m_colorAttachment0_Id, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8UI );
	int VAO = GenericDataSetupForRendering();
	Shader vertex_shader("res/shaders/VS_UAVTest.vert", GL_VERTEX_SHADER);
	Shader fragment_shader("res/shaders/FS_UAVTest.frag", GL_FRAGMENT_SHADER);
	Program program(vertex_shader, fragment_shader);

	fbo2.DisplayFramebufferOnScreen();
	fbo2.BindFramebuffer();
	program.UseProgram();
	glUniform1i(glGetUniformLocation(program.m_program_id, "image0"), 4);
	glBindVertexArray(VAO);
	
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	fbo2.UnbindFramebuffer();
	fbo2.DisplayFramebufferOnScreen();
	const unsigned char* string1 = glGetString(GL_VERSION);
	std::cout << string1;





}



