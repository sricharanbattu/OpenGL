#include "Shaders.h"
#include "fstream"
#include "sstream"
#include <iostream>

Shader::Shader(std::string shader_path, GLenum shader_type) : m_shader_type{shader_type}
{
	m_shader_id = glCreateShader(shader_type);
	std::ifstream Shaderfile;
	Shaderfile.open(shader_path);

	std::stringstream ShaderStream;
	ShaderStream << Shaderfile.rdbuf();
	
	m_shader_code = ShaderStream.str();
	const char* shader_source_code = m_shader_code.c_str();
	glShaderSource(m_shader_id, 1, &shader_source_code, NULL);
}

const char* Shader::GetShaderString() {
	return m_shader_code.c_str();
}

void Shader::PrintShaderCode() {
	std::cout << m_shader_code << '\n';
}

int Shader::CompileShader()
{
	glCompileShader(m_shader_id);
	int success;
	char infoLog[512];
	glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_shader_id, 512, NULL, infoLog);
		if (m_shader_type == GL_VERTEX_SHADER)
		{
			std::cout << "ERROR :: VERTEX SHADER COMPILATION FAILED : " << infoLog << '\n';
		}
		else
		{
			std::cout << "ERROR :: FRAGMENT SHADER COMPILATION FAILED : " << infoLog << '\n';
		}

	}
	return success;
}



Shader::~Shader()
{
	glDeleteShader(m_shader_id);
}


/************************************************************/


Program::Program(Shader vert_shader, Shader frag_shader) : m_vertex_shader{ vert_shader }, m_fragment_shader{ frag_shader }
{
	m_program_id = glCreateProgram();
	glAttachShader(m_program_id, m_vertex_shader.m_shader_id);
	glAttachShader(m_program_id, m_fragment_shader.m_shader_id);

}

int Program::CompileVertexShader()
{
	 return m_vertex_shader.CompileShader();

}

int Program::CompileFragmentShader()
{
	return m_fragment_shader.CompileShader();
}

int Program::CompileShaders()
{
	int vert_shader_success = CompileVertexShader();
	int frag_shader_success = CompileFragmentShader();
	return (vert_shader_success && frag_shader_success);
}

int Program::LinkShaders()
{
	glLinkProgram(m_program_id);
	int program_success;
	char infoLog[512];
	glGetProgramiv(m_program_id, GL_LINK_STATUS, &program_success);
	if (!program_success)
	{
		glGetProgramInfoLog(m_program_id, 512, NULL, infoLog);
		std::cout << "ERROR :: PROGRAM LINKING FAILED : " << infoLog << '\n';
	}

	return program_success;
}

void Program::UseProgram()
{
	int shader_compilation_success = CompileShaders();
	int program_link_success       = LinkShaders();

	glUseProgram(m_program_id);
}

void Program::UnuseProgram()
{
	glUseProgram(0);
}
Program::~Program()
{
	glDeleteProgram(m_program_id);
}