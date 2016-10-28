#pragma once

#include <GLFW/glfw3.h>

class RenderEngine {

public:
	RenderEngine(GLFWwindow* window);
	virtual ~RenderEngine();

	void render();

private:
	GLFWwindow* window;
};

