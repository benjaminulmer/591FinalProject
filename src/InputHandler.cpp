#include "InputHandler.h"

Camera* InputHandler::camera;
RenderEngine* InputHandler::renderEngine;
float InputHandler::mouseOldX;
float InputHandler::mouseOldY;

void InputHandler::setUp(Camera* camera, RenderEngine* renderEngine) {
	InputHandler::camera = camera;
	InputHandler::renderEngine = renderEngine;
}

void InputHandler::key(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

void InputHandler::mouse(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		mouseOldX = x;
		mouseOldY = y;
	}
}

void InputHandler::motion(GLFWwindow* window, double x, double y) {
	double dx, dy;
	dx = (x - mouseOldX);
	dy = (y - mouseOldY);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
	{
		camera->updateLongitudeRotation(dx * 0.5f);
		//rotate_y += dx * 0.5f;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		//translate_z += dy * 0.03f;
	}

	mouseOldX = x;
	mouseOldY = y;
}

void InputHandler::scroll(GLFWwindow* window, double x, double y) {

}

void InputHandler::reshape(GLFWwindow* window, int width, int height) {

}
