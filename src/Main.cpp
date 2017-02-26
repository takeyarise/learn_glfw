#include <iostream>
#include <string>
#include <array>

//#define GLFW_INCLUDE_GLCOREARB
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Program.hpp"
#include "View.hpp"
#include "Perspective.hpp"

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
	glViewport(0, 0, width, height);

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Faild to initialize GLEW" << std::endl;
		return 1;
	}

	glClearColor(0.2, 0.2, 0.2, 1.0);

	// shader
	Program program;
	{
		Shader vertexShader;
		Shader fragmentShader;
		vertexShader.createShader("./resource/vertex.vert", GL_VERTEX_SHADER);
		fragmentShader.createShader("./resource/fragment.frag", GL_FRAGMENT_SHADER);
		program.attachShader(vertexShader);
		program.attachShader(fragmentShader);
		program.linkProgram();
	}

	// create vbo
	std::array<GLfloat, 12> positionData = {
		-1.0f,  0.0f, 0.0f,
		 0.0f, -1.0f, 0.0f,
		 1.0f,  0.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};
	std::array<GLuint, 6> indexData = {
		0, 1, 2,
		0, 2, 3
	};
	std::array<GLfloat, 12> colorData = {
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f
	};

	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, positionData.size() * sizeof(GLfloat), positionData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, colorData.size() * sizeof(GLfloat), colorData.data(), GL_STATIC_DRAW);

	// create ibo
	GLuint iboHandle;
	glGenBuffers(1, &iboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

	// create vao
	GLuint vaoHandle;
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);

	glBindVertexArray(0);

	// gl enable
	glEnable(GL_CULL_FACE);

	// view matrix
	View camera({0.0, 0.0, 5.0});
	camera.setDirectionHorizontalAngle(3.14f);
	camera.setDirectionVerticalAngle(0.0f);

	// perspective matrix
	Perspective perspective;
	perspective.setFieldofView(45.0f);
	perspective.setAspect(static_cast<float>(width) / static_cast<float>(height));
	perspective.setNearClippingPlane(0.1f);
	perspective.setFarClippingPlane(100.0f);

	// get handle shader uniform
	GLuint matrixHandle = glGetUniformLocation(program.getProgramObject(), "MVP");

	// compute the mvp matrix
	glm::mat4 projectionMatrix = perspective.getProjectionMatrix();
	glm::mat4 viewMatrix = camera.getViewMatrix();
	glm::mat4 modelMatrix = glm::mat4(1.0);	// rotate, scale change..
	glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		// ----------
		program.bind();

		glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, &mvpMatrix[0][0]);

		glBindVertexArray(vaoHandle);
		glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		program.unbind();
		// ----------
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
