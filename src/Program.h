#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "InputHandler.h"

class Program {

public:
	Program();
	virtual ~Program();

	void start();

private:
	InputHandler inputHandler;

	int width = 1024;
	int height = 1024;
	GLFWwindow* window;

	static void error(int error, const char* description);
};
