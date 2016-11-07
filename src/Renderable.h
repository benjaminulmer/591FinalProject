#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <forward_list>
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

	void initEdgeBuffer();
	glm::vec3 getDimensions();

private:
	std::vector<std::forward_list<Node>> edgeBuffer;
};

