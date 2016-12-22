#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "ShaderTools.h"
#include "Renderable.h"
#include "Camera.h"
#include "Texture.h"

#include "lodepng.h"

enum class Mode {
	COMBINED,
	ATTRIBUTE,
	IMAGE
};

enum class Attribute {
	ORIENTATION,
	DEPTH
};

class RenderEngine {

public:
	RenderEngine(GLFWwindow* window, Camera* camera);
	virtual ~RenderEngine();

	void render();
	void assignBuffers(Renderable& renderable);
	unsigned int loadTexture(std::string filename);

	void setWindowSize(int width, int height);
	void updateLightPos(glm::vec3 add);
	void setMode(Mode newMode);
	void toggleAttributeMapMode();
	void swapAttributeTexture(int inc);
	void swapDepthTexture(int inc);
	void swapObject(int inc);
	void toggleLineDrawing();
	void updateR(float inc);

	std::vector<GLuint> attributeTextures;
	GLuint attributeID;
	std::vector<GLuint> depthTextures;
	GLuint depthID;
	std::vector<Renderable*> objects;
	unsigned int objectID;

	Attribute attribute;

private:
	GLFWwindow* window;

	GLuint mainProgram;
	GLuint lightProgram;
	GLuint lineProgram;

	//attribute or image mode
	Mode mode;
	bool lineDrawing = true;

	Camera* camera;
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 lightPos;
	float r;

	void renderLines(Renderable& renderable);
	void renderLight();
};

