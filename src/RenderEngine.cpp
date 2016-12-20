#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) :
	activeID(0), window(window), mode(Mode::COMBINED), objectID(0) {

	glfwGetWindowSize(window, &width, &height);

	mainProgram = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");
	lightProgram = ShaderTools::compileShaders("./shaders/light.vert", "./shaders/light.frag");
	lineProgram = ShaderTools::compileShaders("./shaders/line.vert", "./shaders/line.frag");

	lightPos = glm::vec3(0.0, 2.0, 0.0);
	projection = glm::perspective(45.0f, (float)width/height, 0.01f, 100.0f);

	// Default openGL state
	// If you change state you must change back to default after
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.3, 0.4, 0.0);
	glLineWidth(2.0f);
	glPointSize(30.0f);
}

RenderEngine::~RenderEngine() {
	// nothing to do here, program will clean up window pointer
}

// Stub for render call. Will be expanded
void RenderEngine::render() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // TODO Currently done here. Needs to be moved up so only done once per frame

	Renderable& renderable = *objects[objectID];

	glBindVertexArray(renderable.vao);
	glUseProgram(mainProgram);

	//Bind the texture
	int multiply;
	if (attributeTextures[activeID] == 1) {
		multiply = 1;
	}
	else multiply = 0;

	texture.bind2DTexture(mainProgram, attributeTextures[activeID], std::string("attr"));

	if (renderable.textureID == 0) {
		mode = Mode::ATTRIBUTE;
	} // switch to attribute-only mode
	else {
		texture.bind2DTexture(mainProgram, renderable.textureID, std::string("image"));
	}

	glm::mat4 model = glm::mat4();
	glm::mat4 modelView = view * model;

	// Uniforms
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(mainProgram, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1i(glGetUniformLocation(mainProgram, "mode"), (int)mode);
	glUniform1i(glGetUniformLocation(mainProgram, "multiply"), multiply);


	glDrawElements(GL_TRIANGLES, renderable.drawFaces.size(), GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);
	texture.unbind2DTexture();

	renderLines(renderable);
	renderLight();
}

void RenderEngine::renderLines(Renderable& renderable) {
	glUseProgram(lineProgram);
	glBindVertexArray(renderable.edgeVao);

	std::vector<std::list<Node>>& edgeBuffer = renderable.getEdgeBuffer();
	int i = 0;
	for (std::list<Node>& l : edgeBuffer) {
		for (Node& n : l) {
			if ((n.front && n.back) || (n.angle > renderable.getLowerCountour() && n.angle < renderable.getUpperCountour())) {
				glUniformMatrix4fv(glGetUniformLocation(lineProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(lineProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				glDrawArrays(GL_LINES, n.index * 2, 2);
			}
			n.front = false;
			n.back = false;
		}
		i++;
	}
	glBindVertexArray(0);
}

// Renders the current position of the light as a point
void RenderEngine::renderLight() {
	glUseProgram(lightProgram);

	// Uniforms
	glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(lightProgram, "lightPos"), 1, glm::value_ptr(lightPos));

	glDrawArrays(GL_POINTS, 0, 1);
}

// Assigns and binds buffers for a renderable (sends it to the GPU)
void RenderEngine::assignBuffers(Renderable& renderable) {
	std::vector<glm::vec3>& vertices = renderable.drawVerts;
	std::vector<glm::vec3>& normals = renderable.normals;
	std::vector<glm::vec2>& uvs = renderable.uvs;
	std::vector<GLushort>& faces = renderable.drawFaces;

	// Bind attribute array for triangles
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

	// Fill vector with verticies for all edges
	std::vector<std::list<Node>>& edgeBuffer = renderable.getEdgeBuffer();
	std::vector<glm::vec3>& verts = renderable.verts;
	std::vector<glm::vec3> edgeVerts = std::vector<glm::vec3>();

	int i = 0;
	int vertex1 = 0;
	for (std::list<Node>& l : edgeBuffer) {
		for (Node& n : l) {
			edgeVerts.push_back(verts[vertex1]);
			edgeVerts.push_back(verts[n.vertex]);
			n.index = i;
			i++;
		}
		vertex1++;
	}

	// Bind attribute array for edges
	glGenVertexArrays(1, &renderable.edgeVao);
	glBindVertexArray(renderable.edgeVao);

	// Vertex buffer
	glGenBuffers(1, &renderable.edgeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, renderable.edgeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*edgeVerts.size(), edgeVerts.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

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
void RenderEngine::setMode(Mode newMode) {
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
	if ((activeID == 0) && (inc < 0)) {
		activeID = attributeTextures.size() + inc;
	}
	else {
		activeID += inc;
		activeID = activeID % attributeTextures.size();
	}
}

void RenderEngine::swapObject(int inc) {
	if ((objectID == 0) && (inc < 0)) {
		objectID = objects.size() + inc;
	}
	else {
		objectID += inc;
		objectID = objectID % objects.size();
	}
}
