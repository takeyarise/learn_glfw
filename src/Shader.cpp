#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Shader.hpp"

Shader::Shader()
	:shaderId_(0), hasShaderId_(false)
{}

Shader::~Shader()
{
	deleteShader();
}

void Shader::createShader(const char* file, GLenum type)
{
	shaderId_ = glCreateShader(type);

	std::string code;
	readShader(file, code);

	compileShader(code);
}

void Shader::readShader(const char* file, const std::string& shaderCode)
{
	std::ifstream shaderStream(file, std::ios::in);
	if (shaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(shaderStream, line))
		{
			shaderCode += "\n" + line;
		}
		shaderStream.close();
		hasShaderId_ = true;
	}
	else
	{
		std::cerr << "Faild to open the file: " << file << std::endl;
		hasShaderId_ = false;
		return;
	}
}

void Shader::compileShader(const std::string& shaderCode)
{
	if (!hasShaderId_)
	{
		return;
	}

	char const* shaderCodePointer = shaderCode.c_str();
	glShaderSource(shaderId_, 1, &shaderCodePointer, NULL);
	glCompileShader(shaderId_);

	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderId_, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId_, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> shaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderId_, infoLogLength, NULL, &shaderErrorMessage[0]);
		std::cerr << &shaderErrorMessage[0] << std::endl;
	}
}

void Shader::deleteShader()
{
	glDeleteShader(shaderId_);
	shaderId_ = 0;
	hasShaderId_ = false;
}
