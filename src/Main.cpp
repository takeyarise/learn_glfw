#include <iostream>
#include <string>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Program.hpp"
#include "View.hpp"
#include "Perspective.hpp"
#include "Graphic.hpp"

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

	// cube datas
	Cube cube;

	// create vbo
	std::array<GLuint, 2> vboHandles;
	glGenBuffers(vboHandles.size(), vboHandles.data());
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, cube.vertices.size() * sizeof(GLfloat), cube.vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, cube.colors.size() * sizeof(GLfloat), cube.colors.data(), GL_STATIC_DRAW);

	// create ibo
	GLuint iboHandle;
	glGenBuffers(1, &iboHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indices.size() * sizeof(GLuint), cube.indices.data(), GL_STATIC_DRAW);

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
	glm::mat4 modelMatrix = glm::rotate(glm::mat4(), 3.14f / 6, glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(), 3.14f / 4, glm::vec3(0, 1, 0));
	glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		// ----------
		program.bind();

		glUniformMatrix4fv(matrixHandle, 1, GL_FALSE, &mvpMatrix[0][0]);

		glBindVertexArray(vaoHandle);
		glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		program.unbind();
		// ----------
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
