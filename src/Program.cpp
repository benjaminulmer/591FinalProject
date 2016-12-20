#include "Program.h"

Program::Program() {
	window = nullptr;
	renderEngine = nullptr;
	camera = nullptr;
}

Program::~Program() {
	// nothing to do here, end of mainLoop performs clean up
}

// Error callback for glfw errors
void Program::error(int error, const char* description) {
	std::cerr << description << std::endl;
}

// Called to start the program. Conducts set up then enters the main loop
void Program::start() {
	setupWindow();
	GLenum err = glewInit();
	if (glewInit() != GLEW_OK) {
		std::cerr << glewGetErrorString(err) << std::endl;
	}

	renderEngine = new RenderEngine(window);
	camera = new Camera();
	InputHandler::setUp(camera, renderEngine);
	mainLoop();
}

// Creates GLFW window for the program and sets callbacks for input
void Program::setupWindow() {
	glfwSetErrorCallback(Program::error);
	if (glfwInit() == 0) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 16);
	window = glfwCreateWindow(1024, 1024, "Mia & Ben 591", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, InputHandler::key);
	glfwSetMouseButtonCallback(window, InputHandler::mouse);
	glfwSetCursorPosCallback(window, InputHandler::motion);
	glfwSetScrollCallback(window, InputHandler::scroll);
	glfwSetWindowSizeCallback(window, InputHandler::reshape);
}

// Initializes all attribute-based textures
void Program::setupTextures() {
	renderEngine->attributeTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/greyscale.png"));
	renderEngine->attributeTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/fig-10b.png"));
	renderEngine->attributeTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/fig-10c.png"));
	renderEngine->attributeTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/fig-10d.png"));
	renderEngine->attributeTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/fig-11b.png"));
	renderEngine->attributeTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/fig-11c.png"));
	renderEngine->attributeTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/fig-11d.png"));

	renderEngine->activeID = 0;
}

// Main loop
void Program::mainLoop() {
	Renderable* r = ContentLoading::createRenderable("./models/Moblin.obj");
	r->initEdgeBuffer();
	setupTextures();

	r->textureID = (renderEngine->loadTexture("./textures/image/Moblin_body.png"));
	//r->show();
	renderEngine->assignBuffers(*r);

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		renderEngine->setView(camera->getView());

		renderEngine->render(*r);
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	glfwTerminate();
}

