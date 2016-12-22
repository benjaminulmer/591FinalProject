#include "RenderEngine.h"

RenderEngine::RenderEngine(GLFWwindow* window, Camera* camera) :
	window(window),textureMode(TextureMode::MULTIPLICATIVE), attributeMode(AttributeMode::ORIENTATION),
	colourMode(ColourMode::GREYSCALE), lineDrawing(true), r(1.0), camera(camera), orientationID(0), depthID(0), objectID(0) {

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	mainProgram = ShaderTools::compileShaders("./shaders/mesh.vert", "./shaders/mesh.frag");
	lightProgram = ShaderTools::compileShaders("./shaders/light.vert", "./shaders/light.frag");
	lineProgram = ShaderTools::compileShaders("./shaders/line.vert", "./shaders/line.frag");

	lightPos = glm::vec3(0.0, 2.0, 0.0);
	projection = glm::perspective(45.0f, (float)width/height, 0.01f, 100.0f);

	// Default openGL state
	// If you change state you must change back to default after
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glPointSize(30.0f);
	glClearColor(0.3, 0.3, 0.4, 0.0);
}

RenderEngine::~RenderEngine() {
	// nothing to do here, program will clean up window pointer
}

// Called to render the active object. RenderEngine stores all information about how to render
void RenderEngine::render() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	Renderable& renderable = *objects[objectID];
	glBindVertexArray(renderable.vao);
	glUseProgram(mainProgram);

	// Determine which attribute texture to use
	if (attributeMode == AttributeMode::ORIENTATION) {
		if (colourMode == ColourMode::COLOUR) {
			Texture::bind2DTexture(mainProgram, orientationTextures[orientationID], std::string("attr"));
		}
		else {
			Texture::bind2DTexture(mainProgram, orientationTexturesGrey[orientationID], std::string("attr"));
		}
	}
	else {
		if (colourMode == ColourMode::COLOUR) {
			Texture::bind2DTexture(mainProgram, depthTextures[depthID], std::string("attr"));
		}
		else {
			Texture::bind2DTexture(mainProgram, depthTexturesGrey[depthID], std::string("attr"));
		}
	}

	// If the object has no image texture switch to attribute only mode
	if (renderable.textureID == 0) {
		textureMode = TextureMode::ATTRIBUTE;
	}
	else {
		Texture::bind2DTexture(mainProgram, renderable.textureID, std::string("image"));
	}

	view = camera->getLookAt();
	glm::mat4 model = glm::mat4();
	glm::mat4 modelView = view * model;

	// Uniforms
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "modelView"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix4fv(glGetUniformLocation(mainProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(mainProgram, "lightPos"), 1, glm::value_ptr(lightPos));
	glUniform1i(glGetUniformLocation(mainProgram, "texMode"), (int)textureMode);
	glUniform1i(glGetUniformLocation(mainProgram, "attrMode"), (int)attributeMode);
	glUniform1f(glGetUniformLocation(mainProgram, "R"), r);

	glDrawElements(GL_TRIANGLES, renderable.drawFaces.size(), GL_UNSIGNED_SHORT, (void*)0);
	glBindVertexArray(0);
	Texture::unbind2DTexture();

	if (lineDrawing) {
		// Only populate if going to render
		renderable.populateEdgeBuffer(camera->getPosition());
		renderLines(renderable);
	}
	renderLight();
}

// Draws all lines of a renderable as determined by the edge buffer
void RenderEngine::renderLines(Renderable& renderable) {
	glUseProgram(lineProgram);
	glBindVertexArray(renderable.edgeVao);

	std::vector<std::list<Node>>& edgeBuffer = renderable.getEdgeBuffer();
	int i = 0;
	for (std::list<Node>& l : edgeBuffer) {
		for (Node& n : l) {

			// Artist edge only determined by contours bounds. Other ways of determining could be used
			bool artist = (n.angle > renderable.getLowerCountour() && n.angle < renderable.getUpperCountour());
			// if (silhouette || artist edge || boundary edge)
			if ((n.front && n.back) || artist || n.front != n.back) {

				// Calculate size of line
				glm::vec3 camPos = camera->getPosition();
				glm::vec3 point = renderable.verts[n.vertex];
				float distance = glm::length(camPos - point);
				glLineWidth(renderable.getLineThickness() / distance);

				glUniformMatrix4fv(glGetUniformLocation(lineProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(glGetUniformLocation(lineProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
				glDrawArrays(GL_LINES, n.index * 2, 2);
			}
			// Reset the edge buffer flags
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

	unsigned int id = Texture::create2DTexture(_image, _imageWidth, _imageHeight);
	return id;
}

// Sets the (attribute-based) textures that will be used when rendering
void RenderEngine::setTextures(std::vector<GLuint>& orientationTexs, std::vector<GLuint>& orientationTexsGS,
	                           std::vector<GLuint>& depthTexs, std::vector<GLuint>& depthTexsGS) {

	orientationTextures = std::vector<GLuint>(orientationTexs);
	orientationTexturesGrey = std::vector<GLuint>(orientationTexsGS);
	depthTextures = std::vector<GLuint>(depthTexs);
	depthTexturesGrey = std::vector<GLuint>(depthTexsGS);
}

// Sets the objects that will be used when rendering
void RenderEngine::setObjects(std::vector<Renderable*> objs) {
	objects = std::vector<Renderable*>(objs);
}

// Sets projection and viewport for new width and height
void RenderEngine::setWindowSize(int width, int height) {
	projection = glm::perspective(45.0f, (float)width/height, 0.01f, 100.0f);
	glViewport(0, 0, width, height);
}

// Updates lightPos by specified value
void RenderEngine::updateLightPos(glm::vec3 add) {
	lightPos += add;
}

// Switches the way textures are combined when rendering
void RenderEngine::setTextureMode(TextureMode newMode) {
	textureMode = newMode;
	if (textureMode == TextureMode::MULTIPLICATIVE) std::cout << "Combining textures multiplicatively" << std::endl;
	else if (textureMode == TextureMode::ADDITIVE) std::cout << "Combining textures additively" << std::endl;
	else if (textureMode == TextureMode::ATTRIBUTE) std::cout << "Only showing attribute-based texture" << std::endl;
	else if (textureMode == TextureMode::IMAGE) std::cout << "Only showing image-based texture" << std::endl;
}

// Switches between orientation and depth-based attribute mapping modes
void RenderEngine::toggleAttributeMapMode() {
	if (attributeMode == AttributeMode::DEPTH) {
		attributeMode = AttributeMode::ORIENTATION;
		std::cout << "Now using orientation-based textures" << std::endl;
	}
	else {
		attributeMode = AttributeMode::DEPTH;
		std::cout << "Now using depth-based textures" << std::endl;
	}
}

// Switched between colour and greyscale verions of the textures
void RenderEngine::toggleColourMode() {
	if (colourMode == ColourMode::COLOUR) {
		colourMode = ColourMode::GREYSCALE;
		std::cout << "Now using greyscale version of textures" << std::endl;
	}
	else {
		colourMode = ColourMode::COLOUR;
		std::cout << "Now using coloured version of textures" << std::endl;
	}
}

// Changes the active attribute texture
void RenderEngine::swapAttributeTexture(int inc) {
	if (attributeMode == AttributeMode::ORIENTATION) {
		if ((orientationID == 0) && (inc < 0)) {
			orientationID = orientationTextures.size() + inc;
		}
		else {
			orientationID += inc;
			orientationID = orientationID % orientationTextures.size();
		}
	}
	else {
		if ((depthID == 0) && (inc < 0)) {
			depthID = depthTextures.size() + inc;
		}
		else {
			depthID += inc;
			depthID = depthID % depthTextures.size();
		}
	}
}

// Changes the active object to be rendered
Renderable* RenderEngine::swapObject(int inc) {
	if ((objectID == 0) && (inc < 0)) {
		objectID = objects.size() + inc;
	}
	else {
		objectID += inc;
		objectID = objectID % objects.size();
	}
	return objects[objectID];
}

// Toggles line drawing on and off
void RenderEngine::toggleLineDrawing() {
	lineDrawing = !lineDrawing;
	if (lineDrawing == true) std::cout << "Line drawing on" << std::endl;
	else std::cout << "Line drawing off" << std::endl;
}

// Updates the value of r (orientation-mapping)
void RenderEngine::updateR(float inc) {
	r += inc;
	if (r < 0) r = 0;
}
