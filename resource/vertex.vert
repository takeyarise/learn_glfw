#version 410

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 vColor;

//out vec3 Color;

uniform mat4 MVP;

void main(void) {
	//Color = vColor;
	gl_Position = MVP * vec4(position, 1.0);
}
