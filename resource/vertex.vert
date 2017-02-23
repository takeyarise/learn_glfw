#version 410

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vColor;

out vec3 Color;

void main(void) {
	Color = vColor;
	gl_Position = vec4(position, 1.0);
}
