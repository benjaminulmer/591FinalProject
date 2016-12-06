#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) :
	window(window) {

	glfwGetWindowSize(window, &width, &height);

	mainProgram = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");
	lightProgram = ShaderTools::compileShaders("./shaders/light.vert", "./shaders/light.frag");
	lineProgram = ShaderTools::compileShaders("./shaders/line.vert", "./shaders/line.frag");

	lightPos = glm::vec3(0.0, 10.0, 0.0);
	projection = glm::perspective(45.0f, (float)width/height, 0.01f, 100.0f);

	// Default openGL state
	// If you change state you must change back to default after
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.3, 0.4, 0.0);
	glLineWidth(5.0f);
	glPointSize(30.0f);
}

RenderEngine::~RenderEngine() {
	// nothing to do here, program will clean up window pointer
}

// Stub for render call. Will be expanded
void RenderEngine::render(Renderable& renderable) {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // TODO Currently done here. Needs to be moved up so only done once per frame

	glBindVertexArray(renderable.vao);
	glUseProgram(mainProgram);

	glm::mat4 model = glm::mat4();
	glm::mat4 modelView = view * model;

	// Uniforms
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(mainProgram, "lightPos"), 1, glm::value_ptr(lightPos));

	glDrawElements(GL_TRIANGLES, renderable.drawFaces.size(), GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);

	renderLines(renderable);
	renderLight();
}

void RenderEngine::renderLines(Renderable& renderable) {
	std::vector<std::list<Node*>> edgeBuffer = renderable.getEdgeBuffer();
	int i = 0;
	for (std::list<Node*> l : edgeBuffer) {
		for (Node* n : l) {
			if (n->front && n->back) {

				// ALL OF THIS IS TEMPORARY

				std::vector<glm::vec3> verts = std::vector<glm::vec3>();
				verts.push_back(renderable.verts[i]);
				verts.push_back(renderable.verts[n->vertex]);

				GLuint vao, vbo;
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*verts.size(), verts.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
				glEnableVertexAttribArray(0);

				// Draw
				glUseProgram(lineProgram);

				glUniformMatrix4fv(glGetUniformLocation(lineProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(lineProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

				glDrawArrays(GL_LINES, 0, 2);

				// Delete
				glDeleteBuffers(1, &vbo);
				glDeleteVertexArrays(1, &vao);
			}
		}
		i++;
	}
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
