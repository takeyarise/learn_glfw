
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "ProgramObject.hpp"
#include "Shader.hpp"

Program::Program()
	:programId_(glCreateProgram()), hasProgramId_(true)
{}

Program::~Program()
{
	deleteProgram();
}

void Program::attachShader(const Shader& shader)
{
	if (!hasProgramId_)
	{
		return;
	}

	glAttachShader(programId_, shader.getShaderObject());
}

void Program::linkProgram()
{
	if (!hasProgramId_)
	{
		return;
	}

	glLinkProgram(programId_);

	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(programId_, GL_LINK_STATUS, &result);
	glGetProgramiv(programId_, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programId_, infoLogLength, NULL, &programErrorMessage[0]);
		std::cerr << "Error: " << &programErrorMessage[0] << std::endl;
	}
}

GLuint Program::bind()
{
	glUseProgram(programId_);
	return programId_;
}

GLuint Program::unbind()
{
	glUseProgram(0);
	return 0;
}

void Program::deleteProgram()
{
	glDeleteProgram(programId_);
	programId_ = 0;
	hasProgramId_ = false;
}
