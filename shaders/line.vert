#version 430 core

uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vertex;

void main(void) {   
	gl_Position = projection * view * vec4(vertex, 1.0);     
}
