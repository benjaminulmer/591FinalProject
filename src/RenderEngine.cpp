#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) :
	window(window) {

	mainProgram = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");
	lightProgram = ShaderTools::compileShaders("./shaders/light.vert", "./shaders/light.frag");

	lightPos = glm::vec3(10.0, 10, 0.0);
	projection = glm::perspective(45.0f, aspectRatio, 0.01f, 100.0f);

	//create plane geometry
	static const GLfloat quadData[] = {
						-1.0f, 0.0f, -1.0f,
						-1.0f, 0.0f, 1.0f,
						1.0f, 0.0f, -1.0f,
						1.0f, 0.0f, 1.0f,
	};

	//passing model attributes to the GPU
	glGenVertexArrays(1, &planeVertexArray);
	glBindVertexArray(planeVertexArray);

	GLuint planeVertexBuffer;
	glGenBuffers(1, &planeVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (quadData), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
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

void RenderEngine::assignBuffers(Renderable& renderable) {
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	GLuint vao;

	glGenVertexArrays(1, &vao);			//vao
	glGenBuffers(1, &vertexBuffer);		//vertices vbo
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &normalBuffer);		//color vbo
	glGenBuffers(1, &indexBuffer);

	std::vector<glm::vec3> vertices = renderable.verts;
	std::vector<glm::vec2> uvs = renderable.uvs;
	std::vector<glm::vec3> normals = renderable.normals;
	std::vector<unsigned short> faces = renderable.drawFaces;

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_STATIC_DRAW);	//buffering vertex data

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uvs.size(), uvs.data(), GL_STATIC_DRAW);	//buffering uv data

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), normals.data(), GL_STATIC_DRAW);		//buffering normal data

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * faces.size(), faces.data(), GL_STATIC_DRAW);

	//r->setColourVBO(normalBuffer);

	//bind to shaders
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	renderable.vao = vao;
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

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
