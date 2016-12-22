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
	MULTIPLICATIVE,
	ADDITIVE,
	ATTRIBUTE,
	IMAGE
};

enum class AttributeMode {
	ORIENTATION,
	DEPTH
};

enum class ColourMode {
	COLOUR,
	GREYSCALE
};

class RenderEngine {

public:
	RenderEngine(GLFWwindow* window, Camera* camera);
	virtual ~RenderEngine();

	void render();
	void setWindowSize(int width, int height);
	void assignBuffers(Renderable& renderable);
	unsigned int loadTexture(std::string filename);

	void setTextures(std::vector<GLuint>& orientationTexs, std::vector<GLuint>& orientationTexsGS,
		             std::vector<GLuint>& depthTexs, std::vector<GLuint>& depthTexsGS);
	void setObjects(std::vector<Renderable*> objs);

	void updateLightPos(glm::vec3 add);
	void setTextureMode(TextureMode newMode);
	void toggleAttributeMapMode();
	void toggleColourMode();
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
	ColourMode colourMode;
	bool lineDrawing;
	float r;

	Camera* camera;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 lightPos;

	std::vector<GLuint> orientationTextures;
	std::vector<GLuint> orientationTexturesGrey;
	GLuint orientationID;
	std::vector<GLuint> depthTextures;
	std::vector<GLuint> depthTexturesGrey;
	GLuint depthID;
	std::vector<Renderable*> objects;
	unsigned int objectID;

	void renderLines(Renderable& renderable);
	void renderLight();
};

