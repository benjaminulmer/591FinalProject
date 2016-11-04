#include "Renderable.h"

Renderable::Renderable() :
	vao(0), vertexBuffer(0),
	normalBuffer(0), uvBuffer(0),
	indexBuffer(0) {}

Renderable::~Renderable() {
	// Remove data from GPU
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indexBuffer );
	glDeleteVertexArrays(1, &vao);
}

glm::vec3 Renderable::getDimensions() {
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	minX = maxX = verts.at(0).x;
	minY = maxY = verts.at(0).y;
	minZ = maxZ = verts.at(0).z;
	for (unsigned int i = 1; i < verts.size(); i++) {
		glm::vec3 v = verts.at(i);
		minX = glm::min(minX, v.x);
		maxX = glm::max(maxX, v.x);
		minY = glm::min(minY, v.y);
		maxY = glm::max(maxY, v.y);
		minZ = glm::min(minZ, v.z);
		maxZ = glm::max(maxZ, v.z);
	}
	return glm::vec3(glm::abs(maxX-minX), glm::abs(maxY-minY), glm::abs(maxZ-minZ));
}
