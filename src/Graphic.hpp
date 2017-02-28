#pragma once

#include <GL/glew.h>

struct Cube
{
	std::array<GLfloat, 24> vertices = {
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
	};
	std::array<GLuint, 36> indices = {
		0, 3, 1,
		1, 3, 2,
		1, 2, 5,
		5, 2, 6,
		6, 2, 3,
		6, 3, 7,
		7, 3, 0,
		7, 0, 4,
		4, 0, 1,
		4, 1, 5,
		4, 5, 6,
		4, 6, 7
	};
	std::array<GLfloat, 24> colors = {
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};
};
