#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window) :
	window(window) {

	program = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");

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

	glUseProgram(program);

	//scene matrices and camera setup
	glm::vec3 eye(0.0f,0.3f, 2.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	modelView = glm::lookAt(eye, center, up);

	glm::mat4 identity(1.0f);
	projection = glm::perspective(45.0f, aspectRatio, 0.01f, 100.0f);

	//Uniform variables
	glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3fv(glGetUniformLocation(program, "lightPosition"), 1, glm::value_ptr(lightPosition));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}
