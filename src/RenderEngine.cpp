#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) :
	window(window), mode(0), activeID(0) {

	glfwGetWindowSize(window, &width, &height);

	mainProgram = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");
	lightProgram = ShaderTools::compileShaders("./shaders/light.vert", "./shaders/light.frag");

	lightPos = glm::vec3(0.0, 10.0, 0.0);
	projection = glm::perspective(45.0f, (float)width/height, 0.01f, 100.0f);

	// Default openGL state
	// If you change state you must change back to default after
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.3, 0.4, 0.0);
}

RenderEngine::~RenderEngine() {
	// nothing to do here, program will clean up window pointer
}

// Stub for render call. Will be expanded
void RenderEngine::render(const Renderable& renderable) {
	glClear(GL_DEPTH_BUFFER_BIT ); // TODO Currently done here. Needs to be moved up so only done once per frame
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(renderable.vao);
	glUseProgram(mainProgram);

	//bind the texture
	texture.bind2DTexture(mainProgram, attributeTextures[activeID], std::string("attr"));
	if (renderable.textureID == NULL) { mode = 1; } // switch to attribute-only mode
	else { texture.bind2DTexture(mainProgram, renderable.textureID, std::string("image")); }

	glm::mat4 model = glm::mat4();
	glm::mat4 modelView = view * model;

	// Uniforms
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(mainProgram, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1i(glGetUniformLocation(mainProgram, "mode"), mode);


	glDrawElements(GL_TRIANGLES, renderable.drawFaces.size(), GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);

	//unbind the texture
	texture.unbind2DTexture();

	renderLight();
}

// Renders the current position of the light as a point
void RenderEngine::renderLight() {
	glUseProgram(lightProgram);

	// Uniforms
	glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(lightProgram, "lightPos"), 1, glm::value_ptr(lightPos));

	glPointSize(30.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

// Assigns and binds buffers for a renderable (sends it to the GPU)
void RenderEngine::assignBuffers(Renderable& renderable) {
	std::vector<glm::vec3> vertices = renderable.drawVerts;
	std::vector<glm::vec3> normals = renderable.normals;
	std::vector<glm::vec2> uvs = renderable.uvs;
	std::vector<GLushort> faces = renderable.drawFaces;

	// Bind attribute array
	glGenVertexArrays(1, &renderable.vao);
	glBindVertexArray(renderable.vao);

	// Vertex buffer
	glGenBuffers(1, &renderable.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderable.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Normal buffer
	glGenBuffers(1, &renderable.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderable.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// UV buffer
	glGenBuffers(1, &renderable.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderable.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uvs.size(), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	// Face buffer
	glGenBuffers(1, &renderable.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*faces.size(), faces.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

// Sets view matrix to new value provided
void RenderEngine::setView(const glm::mat4& newView) {
	view = newView;
}

// Sets projection and viewport for new width and height
void RenderEngine::setWindowSize(int width, int height) {
	this->width = width;
	this->height = height;
	projection = glm::perspective(45.0f, (float)width/height, 0.01f, 100.0f);
	glViewport(0, 0, width, height);
}

// Updates lightPos by specified value
void RenderEngine::updateLightPos(glm::vec3 add) {
	lightPos += add;
}

// Switches between attribute and image-based texturing modes
void RenderEngine::setMode(GLuint newMode) {
	mode = newMode;
}

// Creates a 2D texture
unsigned int RenderEngine::loadTexture(std::string filename) {
	//reading model texture image
	std::vector<unsigned char> _image;
	unsigned int _imageWidth, _imageHeight;

	unsigned int error = lodepng::decode(_image, _imageWidth, _imageHeight, filename.c_str());
	if (error)
	{
		std::cout << "reading error" << error << ":" << lodepng_error_text(error) << std::endl;
	}

	unsigned int id = texture.create2DTexture(_image, _imageWidth, _imageHeight);
	return id;
}

void RenderEngine::swapAttributeTexture(int inc) {
	activeID += inc;
	activeID = activeID % attributeTextures.size();
}
