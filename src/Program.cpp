#include "Program.h"

#include <iostream>

Program::Program() {
	// TODO Auto-generated constructor stub

}

Program::~Program() {
	// TODO Auto-generated destructor stub
}

void Program::error(int error, const char* description) {

}

void Program::start() {
	inputHandler = InputHandler();

	glfwSetErrorCallback(Program::error);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, InputHandler::key);
	glfwSetMouseButtonCallback(window, InputHandler::mouse);
	glfwSetCursorPosCallback(window, InputHandler::motion);
	glfwSetScrollCallback(window, InputHandler::scroll);
	glfwSetWindowSizeCallback(window, InputHandler::reshape);

	int n;
	std::cin >> n;
}
