#pragma once

#include <GLFW/glfw3.h>

class InputHandler {

public:
	static void key(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse(GLFWwindow* window, int button, int action, int mods);
	static void motion(GLFWwindow* w, double x, double y);
	static void scroll(GLFWwindow* w, double x, double y);
	static void reshape(GLFWwindow* w, int widthX, int heightY);
};
