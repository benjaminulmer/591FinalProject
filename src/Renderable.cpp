#include "Renderable.h"
#include <iostream>

Renderable::Renderable() :
	vao(0), vertexBuffer(0),
	normalBuffer(0), uvBuffer(0),
	indexBuffer(0){}

Renderable::~Renderable() {
	// Remove data from GPU
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indexBuffer );
	glDeleteVertexArrays(1, &vao);
}

// Initialize edge buffer data structure
void Renderable::initEdgeBuffer() {
	edgeBuffer = std::vector<std::forward_list<Node>>(verts.size() - 1);
	for (std::forward_list<Node> l : edgeBuffer) {
		l = std::forward_list<Node>();
	}

	for (unsigned int i = 0; i < faces.size(); i += 3) {
		std::vector<unsigned int> faceVerts = std::vector<unsigned int>(3);
		faceVerts[0] = faces[i];
		faceVerts[1] = faces[i+1];
		faceVerts[2] = faces[i+2];

		std::sort(faceVerts.begin(), faceVerts.end());
		std::cout << faceVerts[0] << " : " << faceVerts[1] << " : " << faceVerts[2] << std::endl;
	}

}

// Returns dimensions of object (bounding box dimensions)
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
