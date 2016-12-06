#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <algorithm>

struct Node {
	Node(int vertex) : vertex(vertex) {};

	unsigned short vertex;
	bool front = 0;
	bool back = 0;
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

	unsigned int textureID;

	void initEdgeBuffer();
	glm::vec3 getDimensions();

	void show();

private:
	std::vector<std::list<Node>> edgeBuffer;

	void insertEdge(unsigned int vertex1, unsigned int vertex2);
};

