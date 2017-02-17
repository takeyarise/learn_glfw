#pragma once

class Shader;

class Program
{
public:
	Program();
	~Program();
	Program(const Program& other) = delete;
	Program& operator=(const Program& other) = delete;

	void attachShader(const Shader& shader);
	void linkProgram();
	GLuint getProgramObject() const { return programId_; }
	GLuint bind();
	GLuint unbind();
private:
	void deleteProgram();

	GLuint programId_;
	bool hasProgramId_;
};
