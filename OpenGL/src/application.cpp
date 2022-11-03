/*

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#define ASSERT(x) if(!(x)) __debugbreak();

#ifdef _DEBUG
    #define GLCall(x) GLClearError();\
                     x;\
                 ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

static void GLClearError() {

    while (glGetError() != GL_NO_ERROR);
    
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[ OpenGL Error] : (" << error << ")" << function 
            << " " << file << ':' << line <<'\n';
        return false;
    }
    return true;
}

enum class ShaderType {
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1

};

struct ShaderProgramSources {
    std::string vertex_source;
    std::string fragment_source;
};

static ShaderProgramSources parseShader(const std::string& file_path) 
{
    std::ifstream stream(file_path);
    std::string line;
    std::stringstream ss[2];
    ShaderType shader_type = ShaderType::NONE;

    while (getline(stream,line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                //set mode to vertex
                shader_type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                shader_type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)shader_type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
    
}


static unsigned int compileShader(GLenum shader_type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(shader_type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    //Error handling
    int iv;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &iv));
    if (iv == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // dynamically allocates in stack. A cool relatively unknown thing.
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << '\n';
        std::cout << message << '\n';

        free(message);
        GLCall(glDeleteShader(id));
        return 0;

    }
    //std::cout << "Entered compileShader\n";

    return id;
}

static unsigned int createShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader) 
{

    //std::cout << "Entered createShader\n";
    GLCall(unsigned int  program = glCreateProgram());
    unsigned int vs_id = compileShader(GL_VERTEX_SHADER, vertex_shader);
    unsigned int fs_id = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

    GLCall(glAttachShader(program, vs_id));
    GLCall(glAttachShader(program, fs_id));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs_id));
    GLCall(glDeleteShader(fs_id)); // These are no longer required as we have linked them with program


    return program;

}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;

    //if (glewInit() != GLEW_OK)
     //   std::cout << "GLEW_OK did not occur";

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW_OK2 did not occur";

    std::cout << glGetString(GL_VERSION) << '\n';

    unsigned int buffer;
    double positions[]{0.5, 0.5, 
                        0.5, -0.5, 
                        -0.5, 0.5,
                        -0.5, -0.5
                        };

    unsigned int buffer_index;
    unsigned int indices[]{0,1,2,2,1,3};


    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 8*sizeof(double), positions, GL_STATIC_DRAW));

    GLCall(glGenBuffers(1, &buffer_index));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_index));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2*sizeof(double), 0));

    

    

    // Note that the path is taken relative to the debugging path(which is project directory), not the current directory
    ShaderProgramSources shader_sources = parseShader("res/shaders/Basicshader.shader");
    std::string vertex_shader = shader_sources.vertex_source;
    std::string fragment_shader = shader_sources.fragment_source; 

    unsigned int shader = createShaderProgram(vertex_shader, fragment_shader);
    GLCall(glUseProgram(shader));

    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    
    float r = 0.0f;
    float increment=0.05f;

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here 
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glUniform4f(location, r, 0.2f, 0.4f, 1.0f));
       //glDrawArrays(GL_TRIANGLES, 0, 3);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;
        //Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events 
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));
    glfwTerminate();
    return 0;
}

*/