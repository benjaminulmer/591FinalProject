#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderTools.h"

class RenderEngine {

public:
	RenderEngine(GLFWwindow* window);
	virtual ~RenderEngine();

	void render();
	void updateView(glm::mat4 value);
	void updateAspectRatio();

private:
	GLFWwindow* window;

	GLuint mainProgram;
	GLuint lightProgram;

	// Temp
	GLuint planeVertexArray;
	//

	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 lightPos;

	float aspectRatio = 1;

	void renderLight();
};

