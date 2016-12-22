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

enum class TextureMode {
	COMBINED,
	ATTRIBUTE,
	IMAGE
};

enum class AttributeMode {
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

	void setTextures(std::vector<GLuint>& orientationTexs, std::vector<GLuint>& depthTexs);
	void setObjects(std::vector<Renderable*> objs);

	void setWindowSize(int width, int height);
	void updateLightPos(glm::vec3 add);
	void setMode(TextureMode newMode);
	void toggleAttributeMapMode();
	void swapAttributeTexture(int inc);
	Renderable* swapObject(int inc);
	void toggleLineDrawing();
	void updateR(float inc);

private:
	GLFWwindow* window;

	GLuint mainProgram;
	GLuint lightProgram;
	GLuint lineProgram;

	//attribute or image mode
	TextureMode textureMode;
	AttributeMode attributeMode;
	bool lineDrawing;
	float r;

	Camera* camera;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 lightPos;

	std::vector<GLuint> orientationTextures;
	GLuint orientationID;
	std::vector<GLuint> depthTextures;
	GLuint depthID;
	std::vector<Renderable*> objects;
	unsigned int objectID;

	void renderLines(Renderable& renderable);
	void renderLight();
};

