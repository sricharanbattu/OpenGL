
/*#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Shader_class.h"

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwInit();
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Unable to Open a Window" << '\n';
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);

    if (glewInit() != GLEW_OK)
        std::cout << "Unable to load GLEW" << '\n';

    glViewport(0, 0, 800, 600);

    
    ShaderCodes shader_codes("res/shaders/vertex_shader.vert", "res/shaders/frag_shader.frag");
    const char* vertex_source = shader_codes.vertex_code.c_str();
    const char* frag_source = shader_codes.frag_code.c_str();

    


    /*const char* vertex_source = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n" 
        "layout (location = 2) in vec2 aTex;\n"
        "uniform float cos;\n"
        "uniform float sin;\n"
        "out vec4 outColor;\n"
        "out vec2 outTexcord; \n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x*cos, aPos.y, -1*aPos.x*sin, 1.0);\n"
        "   outColor = vec4(aColor, 1.0);\n"
        "   outTexcord = aTex;\n"
        "}\0";

    const char* frag_source = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec4 outColor;\n"
        "in vec2 outTexcord;\n"
        "uniform sampler2D ourTexture;\n"
        "void main()\n"
        "{\n"
        "  //FragColor = texture(ourTexture, outTexcord);\n"
        "  FragColor = outColor*texture(ourTexture, outTexcord);\n"
        "  //FragColor = outColor;\n"
        "}\0";

    
    unsigned int vertex_shader, frag_shader;

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
    glCompileShader(vertex_shader);

    int success;
    char infolog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infolog);
        std::cout << "Compilation of Vertex shader failed" << infolog << '\n';
    }

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_source, nullptr);
    glCompileShader(frag_shader);
    
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(frag_shader, 512, nullptr, infolog);
        std::cout << "Compilation of Fragment shader failed" << infolog << '\n';
    }

    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, frag_shader);

    glLinkProgram(shader_program);
    glValidateProgram(shader_program);
    glGetProgramiv(shader_program, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_program, 512, nullptr, infolog);
        std::cout << "Program failed" << infolog << '\n';
    }


    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    int cosLocation = glGetUniformLocation(shader_program, "cos");
    int sinLocation = glGetUniformLocation(shader_program, "sin");
    

    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    
   
    
    
    




    float vertices[] = {
        -0.5f, -0.5f,0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.5f, 0.5f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 4

    };

    unsigned int VBO, VAO, EBO;
    unsigned int texture;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenTextures(1, &texture);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindTexture(GL_TEXTURE_2D, texture);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    if (data)
        glGenerateMipmap(GL_TEXTURE_2D);
    else
        std::cout << "Failed to load the texture" << '\n';
    stbi_image_free(data);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
   
    float theta_res = 3.14;
    const float pi = 3.141592;
    float theta = 0;

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader_program);

        theta += theta_res;
        if (theta > 2 * pi)
            theta -= 2 * pi;

        glUniform1f(cosLocation, std::cos(theta));
        glUniform1f(sinLocation, std::sin(theta));


        glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader_program);
    glfwTerminate();
    return 0;
}
*/