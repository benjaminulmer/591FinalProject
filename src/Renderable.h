#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <algorithm>

enum class Facing {
	FRONT,
	BACK
};

struct Node {
	Node(int vertex) : vertex(vertex) {};

	unsigned short vertex;
	bool front = false;
	bool back = false;
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

	void initEdgeBuffer();
	void populateEdgeBuffer(glm::vec3 eye);
	void clearEdgeBuffer();
	std::vector<std::list<Node>>& getEdgeBuffer() {return edgeBuffer;};
	glm::vec3 getDimensions();

	void show();

private:
	std::vector<std::list<Node>> edgeBuffer;

	void insertEdge(unsigned int vertex1, unsigned int vertex2);
	void updateEdge(unsigned int vertex1, unsigned int vertex2, Facing);
};

