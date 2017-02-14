#include <iostream>

#include <GLFW/glfw3.h>

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cerr << "can't initialize glfw" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		std::cerr << "can't create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glClearColor(0.2, 0.2, 0.2, 1.0);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
