#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) :
	window(window) {

	mainProgram = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");
	lightProgram = ShaderTools::compileShaders("./shaders/light.vert", "./shaders/light.frag");

	lightPos = glm::vec3(0.0, 0.8, 0.0);
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
void RenderEngine::render() {
	glBindVertexArray(planeVertexArray);
	glUseProgram(mainProgram);

	glm::mat4 model = glm::mat4();
	glm::mat4 modelView = view * model;

	//Uniform variables
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(mainProgram, "lightPos"), 1, glm::value_ptr(lightPos));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	renderLight();
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
void RenderEngine::updateView(glm::mat4 newView) {
	view = newView;
}

// Updates aspectRatio to new value provided
void RenderEngine::updateAspectRatio() {
	//TODO do this
}
