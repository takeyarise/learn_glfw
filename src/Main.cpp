#include <iostream>
#include <string>

//#define GLFW_INCLUDE_GLCOREARB
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Program.hpp"

int main()
{
	const int width = 640;
	const int height = 480;
	const std::string title = "Hello openGL";

	if (!glfwInit())
	{
		std::cerr << "can't initialize glfw" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window;
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window)
	{
		std::cerr << "can't create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Faild to initialize GLEW" << std::endl;
		return 1;
	}

	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClearDepth(1.0);

	// shader
	Program program;
	{
		Shader vertexShader;
		Shader fragmentShader;
		vertexShader.createShader("./resource/vertex.vert", GL_VERTEX_SHADER);
		fragmentShader.createShader("./resource/fragment.frag", GL_FRAGMENT_SHADER);
		program.attachShader(vertexShader);
		program.attachShader(fragmentShader);
	}
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
