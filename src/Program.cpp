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

	camera = new Camera();
	renderEngine = new RenderEngine(window, camera);
	InputHandler::setUp(camera, renderEngine);
	setupTextures();
	loadObjects();
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
	std::vector<GLuint> orientationTextures;
	orientationTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/colour/fig-10b.png"));
	orientationTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/colour/fig-10c.png"));
	orientationTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/colour/fig-10d.png"));
	orientationTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/colour/fig-11b.png"));
	orientationTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/colour/fig-11c.png"));
	orientationTextures.push_back(renderEngine->loadTexture("./textures/attribute/orientation/colour/fig-11d.png"));

	std::vector<GLuint> orientationTexturesGrey;
	orientationTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/orientation/greyscale/fig-10b.png"));
	orientationTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/orientation/greyscale/fig-10c.png"));
	orientationTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/orientation/greyscale/fig-10d.png"));
	orientationTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/orientation/greyscale/fig-11b.png"));
	orientationTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/orientation/greyscale/fig-11c.png"));
	orientationTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/orientation/greyscale/fig-11d.png"));

	std::vector<GLuint> depthTextures;
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-7b.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-7c.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-7d.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-8a.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-8b.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-9b.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-9c.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-9e.png"));
	depthTextures.push_back(renderEngine->loadTexture("./textures/attribute/depth/colour/fig-9f.png"));

	std::vector<GLuint> depthTexturesGrey;
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-7b.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-7c.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-7d.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-8a.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-8b.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-9b.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-9c.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-9e.png"));
	depthTexturesGrey.push_back(renderEngine->loadTexture("./textures/attribute/depth/greyscale/fig-9f.png"));

	renderEngine->setTextures(orientationTextures, orientationTexturesGrey, depthTextures, depthTexturesGrey);
}

// Loads and initializes all objects that can be viewed
void Program::loadObjects() {
	std::vector<Renderable*> objects;

	Renderable* o = ContentLoading::createRenderable("./models/Moblin.obj");
	o->textureID = (renderEngine->loadTexture("./textures/image/Moblin.png"));
	objects.push_back(o);

	o = ContentLoading::createRenderable("./models/Bokoblin.obj");
	o->textureID = (renderEngine->loadTexture("./textures/image/Bokoblin.png"));
	objects.push_back(o);

	o = ContentLoading::createRenderable("./models/Helmaroc.obj");
	o->textureID = (renderEngine->loadTexture("./textures/image/Helmaroc.png"));
	objects.push_back(o);

	// These two don't have textures associated with the models
	objects.push_back(ContentLoading::createRenderable("./models/Tree.obj"));
	objects.push_back(ContentLoading::createRenderable("./models/Castle.obj"));

	// Send each object to GPU and initialize edge buffer
	for (Renderable* r : objects) {
		r->initEdgeBuffer();
		renderEngine->assignBuffers(*r);
	}

	InputHandler::setCurRenderable(objects[0]);
	renderEngine->setObjects(objects);
}

// Main loop
void Program::mainLoop() {
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		renderEngine->render();
		glfwSwapBuffers(window);
	}

	// Clean up, program needs to exit
	glfwDestroyWindow(window);
	glfwTerminate();
}
