#pragma once

#include <string>
#include "include.hpp"

class Shader
{
public:
	Shader();
	~Shader();
	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	void createShader(const char* shaderFile, GLenum type)
	GLuint getShaderObject() const { return shaderId_; }
private:
	void readShader(const char* shaderFile, const std::string& code);
	void compileShader(const std::string& code);
	void deleteShader();

	GLuint shaderId_;
	bool hasShaderId_;
};
