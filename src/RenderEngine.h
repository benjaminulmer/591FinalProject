#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderTools.h"
#include "Renderable.h"

#include "texture.h"
#include "lodepng.h"

class RenderEngine {

public:
	RenderEngine(GLFWwindow* window);
	virtual ~RenderEngine();

	void render(const Renderable& renderable);
	void assignBuffers(Renderable& renderable);
	void setView(const glm::mat4& value);
	void setWindowSize(int width, int height);
	void updateLightPos(glm::vec3 add);

	unsigned int loadTexture(std::string filename);

private:
	GLFWwindow* window;
	int width;
	int height;

	GLuint mainProgram;
	GLuint lightProgram;

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 lightPos;

	Texture texture;

	void renderLight();
};

