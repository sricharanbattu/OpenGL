#pragma once

# include <string>
#include "import_gl.h"
class Shader {
public:
	unsigned int m_shader_id;
	GLenum m_shader_type;
	std::string m_shader_code;


	Shader(std::string shader_path, GLenum shader_type);
	~Shader();
	const char* GetShaderString();
	void PrintShaderCode();
	int CompileShader();

    
};

class Program {
public:
	unsigned int m_program_id;
	Shader& m_vertex_shader;
	Shader& m_fragment_shader;

	Program(Shader vert_shader, Shader frag_shader);
	int CompileShaders();
	int CompileVertexShader();
	int CompileFragmentShader();
	int LinkShaders();
	void UseProgram();
	void UnuseProgram();
	~Program();
};
