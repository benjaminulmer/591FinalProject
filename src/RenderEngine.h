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

private:
	GLFWwindow* window;

	GLuint program;
	GLuint planeVertexArray;

	glm::mat4 modelView;
	glm::mat4 projection;
	glm::vec3 lightPosition;

	float aspectRatio = 1;
};

