#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h>

class Renderable
{
public:
	Renderable();
	virtual ~Renderable();

	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;
	std::vector<GLushort> drawFaces;
	std::vector<GLuint> faces; // TODO kill
	std::vector<glm::vec3> raw_verts;
	GLuint vbo;
	GLuint vao;
	GLuint indexBuffer;

	glm::vec3 getDimensions();
};

