#include <iostream>

//#define GLFW_DLL
//#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "ProgramObject.hpp"

int main()
{
	GLFWwindow* window;
	const int width = 640;
	const int height = 480;

	if (!glfwInit())
	{
		std::cerr << "can't initialize glfw" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cerr << "can't create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClearDepth(1.0);

	// shader
	Shader vertexShader;
	Shader fragmentShader;
	vertexShader.createShader("./resource/vertex.vert");
	fragmentShader.createShader("./resource/fragment.frag");
	// program
	Program program;
	program.attachShader(vertexShader);
	program.attachShader(fragmentShader);
	program.linkProgram();

	// triangle
	GLint attLocation = glGetAttribLocation(program.getProgramObject(), "position");
	int attStride = 3;
	GLfloat vertexPos[] = {
		0.0, 1.0, 0.0,
		1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0
	};
	// create vbo

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
