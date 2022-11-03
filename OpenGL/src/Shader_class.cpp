#include "Shader_class.h"
#include "fstream"
#include "sstream"

ShaderCodes::ShaderCodes(std::string vert_path, std::string frag_path) {
	std::ifstream vShaderfile;
	std::ifstream fShaderfile;

	vShaderfile.open(vert_path);
	fShaderfile.open(frag_path);

	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << vShaderfile.rdbuf();
	fShaderStream << fShaderfile.rdbuf();
	 
	vertex_code = vShaderStream.str();
	frag_code = fShaderStream.str();


}

