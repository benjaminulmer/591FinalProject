#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

enum class Facing {
	FRONT,
	BACK
};

struct Node {
	Node(int vertex) : vertex(vertex) {};

	unsigned short vertex;
	unsigned short angle = 0;
	bool front = false;
	bool back = false;

	GLuint index = 0;
};

class Renderable {

public:
	Renderable();
	virtual ~Renderable();

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> drawVerts;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<GLushort> faces;
	std::vector<GLushort> drawFaces;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint indexBuffer;
	GLuint textureID;

	GLuint edgeVao;
	GLuint edgeVertexBuffer;

	void updateContourBounds(int lower, int upper);
	void updateLineThickness(float inc);
	int getLowerCountour() {return contourLower;}
	int getUpperCountour() {return contourUpper;}
	float getLineThickness() {return lineThickness;}
	void initEdgeBuffer();
	void populateEdgeBuffer(glm::vec3 eye);
	std::vector<std::list<Node>>& getEdgeBuffer() {return edgeBuffer;};

	void show();

private:
	std::vector<std::list<Node>> edgeBuffer;
	int contourLower = 65;
	int contourUpper = 165;
	float lineThickness = 8.0f;

	void insertEdge(unsigned int vertex1, unsigned int vertex2, std::vector<std::list<glm::vec3>>& tempBuffer, glm::vec3 normal);
	void updateEdge(unsigned int vertex1, unsigned int vertex2, Facing);
};

