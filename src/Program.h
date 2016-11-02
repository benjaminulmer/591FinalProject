#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "InputHandler.h"
#include "RenderEngine.h"
#include "Renderable.h"
#include "ContentLoading.h"

class Program {

public:
	Program();
	virtual ~Program();

	void start();

private:
	GLFWwindow* window;
	RenderEngine* renderEngine;

	static void error(int error, const char* description);
	void setupWindow();
	void mainLoop();
};
