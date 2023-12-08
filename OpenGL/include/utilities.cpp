#include "import_gl.h"
#include "utilities.h"
#include "Texture.h"
#include "Shaders.h"

void DisplayTextureOnScreen(unsigned int* pTextureId, GLFWwindow* window)
{
	
	glBindTexture(GL_TEXTURE_2D, *pTextureId);
	int texwidth,texheight, nChannels;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texwidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texheight);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH, &nChannels);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (window == nullptr)
	{
		std::cout << "Unable to Open current window" << '\n';
		//glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

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

	std::string vert_shader_path = "res/shaders/VS_passPosAndTex.vert";
	std::string frag_shader_path = "res/shaders/FS_SampleTexture.frag";

	Shader shader1(vert_shader_path, GL_VERTEX_SHADER);
	Shader shader2(frag_shader_path, GL_FRAGMENT_SHADER);

	Program program1(shader1, shader2);
	program1.CompileShaders();
	program1.LinkShaders();
	program1.UseProgram();

	
	glUniform1i(glGetUniformLocation(program1.m_program_id, "ourTexture"), 0);
	glDeleteShader(shader1.m_shader_id);
	glDeleteShader(shader2.m_shader_id);

	
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(window))
	{
		//glUseProgram(program1.m_program_id);
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *pTextureId);
		glUniform1i(glGetUniformLocation(program1.m_program_id, "ourTexture"), 0);



		glClearColor(1.0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//glfwTerminate();


}
void DisplayDataOnScreen(void* textureData, int width, int height, int nChannels)
{
	GLFWwindow* current_context_window = glfwGetCurrentContext();
	GLFWwindow* textureDisplayWindow = glfwCreateWindow(width, height, "DisplayOutOfContext", nullptr, nullptr);
	if (textureDisplayWindow == nullptr)
	{
		std::cout << "Unable to Open TextureDisplayWindow" << '\n';
		//glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(textureDisplayWindow);
	glViewport(0, 0, width, height);
	

	//unsigned int texture_id;
	Texture2D textureInfo = Texture2D(textureData, width, height, nChannels);
	textureInfo.BindAndAssignTexture2DData();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	DisplayTextureOnScreen(&textureInfo.m_texture_id, textureDisplayWindow);
	
	glfwMakeContextCurrent(current_context_window);

}

void DisplayAnFboOnScreen(unsigned int fbo_id, int width, int height, int nChannels)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	unsigned char* fbodata = new unsigned char[width * height * nChannels];
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, fbodata);
	//for (int i = 0; i < width * height * nChannels; i++)
	//{
	//	std::cout << fbodata[i] << "; ";
	//}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	DisplayDataOnScreen(fbodata, width, height, nChannels);
	delete[] fbodata;
}


void RenderTexture2DDataOntoFBO(Framebuffer& fbo, Texture2D& tex)
{
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

	std::string vert_shader_path = "res/shaders/VS_passPosAndTex.vert";
	std::string frag_shader_path = "res/shaders/FS_SampleTexture.frag";

	Shader shader1(vert_shader_path, GL_VERTEX_SHADER);
	Shader shader2(frag_shader_path, GL_FRAGMENT_SHADER);

	Program program1(shader1, shader2);
	program1.CompileShaders();
	program1.LinkShaders();
	program1.UseProgram();


	glUniform1i(glGetUniformLocation(program1.m_program_id, "ourTexture"), 0);
	glDeleteShader(shader1.m_shader_id);
	glDeleteShader(shader2.m_shader_id);


	glBindVertexArray(0);

	glBindVertexArray(VAO);
	fbo.BindFramebuffer();
	tex.BindToTexture2D();

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	fbo.UnbindFramebuffer();
	program1.UnuseProgram();

}

unsigned int GenericDataSetupForRendering()
{
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


	return VAO;
}