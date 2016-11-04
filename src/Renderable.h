#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Renderable
{
public:
	Renderable();
	virtual ~Renderable();

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<GLushort> drawFaces;
	std::vector<GLuint> faces; // TODO kill
	std::vector<glm::vec3> rawVerts;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint uvBuffer;
	GLuint indexBuffer;

	glm::vec3 getDimensions();
};

