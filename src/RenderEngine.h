#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

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
	void setMode(GLuint newMode);

	unsigned int loadTexture(std::string filename);
	void swapAttributeTexture(int inc);

	std::vector<GLuint> attributeTextures;
	unsigned int activeID;

private:
	GLFWwindow* window;
	int width;
	int height;

	GLuint mainProgram;
	GLuint lightProgram;

	//attribute or image mode
	unsigned int mode;

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 lightPos;

	Texture texture;

	void renderLight();
};

