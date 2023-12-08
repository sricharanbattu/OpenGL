#include "../include/import_gl.h"

#include "../include/Shaders.h"
//#include "../include/Texture.h"

#include "Test2_UAV.h"
#include "Test2_UAV_utilities.h"
#include "Test2_UAV_unitTests.h"









int main()
{
	glfwInit();
	int gWIDTH = 800;
	int gHEIGHT = 600;


	int width = gWIDTH;
	int height = gHEIGHT;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window1 = glfwCreateWindow(width, height, "Test2_UAV", nullptr, nullptr);

	

	if (window1 == nullptr)
	{
		std::cout << "Unable to Open a Window" << '\n';
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window1);

	if (glewInit() != GLEW_OK)
		std::cout << "Unable to load GLEW" << '\n';

	glViewport(0, 0, width, height);

	//UnitTest2_FBODisplayTest();
	//UnitTest1_TextureCreateTest();
	UnitTest3_UAVTest();
	glfwTerminate();

	return 0; 


}

