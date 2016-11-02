#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) :
	window(window) {

	mainProgram = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");
	lightProgram = ShaderTools::compileShaders("./shaders/light.vert", "./shaders/light.frag");

	lightPos = glm::vec3(10.0, 10, 0.0);
	projection = glm::perspective(45.0f, aspectRatio, 0.01f, 100.0f);
}

RenderEngine::~RenderEngine() {
	// TODO Auto-generated destructor stub
}

// Stub for render call. Will be expanded
void RenderEngine::render(const Renderable& renderable) {

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT );

	glBindVertexArray(renderable.vao);
	glUseProgram(mainProgram);

	glm::mat4 model = glm::mat4();
	glm::mat4 modelView = view * model;

	//Uniform variables
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(mainProgram, "lightPos"), 1, glm::value_ptr(lightPos));

	glDrawElements(GL_TRIANGLES, renderable.drawFaces.size(), GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);

	renderLight();
}

// Assigns and binds buffers for a renderable (sends it to the GPU)
void RenderEngine::assignBuffers(Renderable& renderable) {
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint indexBuffer;

	std::vector<glm::vec3> vertices = renderable.verts;
	std::vector<glm::vec3> normals = renderable.normals;
	std::vector<glm::vec2> uvs = renderable.uvs;
	std::vector<unsigned short> faces = renderable.drawFaces;

	// Bind attribute array
	glGenVertexArrays(1, &renderable.vao);
	glBindVertexArray(renderable.vao);

	// Vertex buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);	//buffering vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// Normal buffer
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);		//buffering normal data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// UV buffer
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uvs.size(), uvs.data(), GL_STATIC_DRAW);	//buffering uv data
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	// Face buffer
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * faces.size(), faces.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

// Renders the current position of the light as a point
void RenderEngine::renderLight() {
	glUseProgram(lightProgram);

	//uniform variables
	glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(lightProgram, "lightPos"), 1, glm::value_ptr(lightPos));

	glPointSize(30.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

// Updates view matrix to new value provided
void RenderEngine::updateView(const glm::mat4& newView) {
	view = newView;
}

// Updates aspectRatio to new value provided
void RenderEngine::updateAspectRatio() {
	//TODO do this
}
