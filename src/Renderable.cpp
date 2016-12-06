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

// Inserts edge into edge buffer if it is not already present
void Renderable::insertEdge(unsigned int vertex1, unsigned int vertex2) {
	bool found = false;
	for (Node* n : edgeBuffer[vertex1]) {
		if (n->vertex == vertex2) {
			found = true;
			break;
		}
	}
	if (!found) {
		edgeBuffer[vertex1].push_back(new Node(vertex2));
	}
}

// Update specified bit field for edge in edge buffer
void Renderable::updateEdge(unsigned int vertex1, unsigned int vertex2, Facing facing) {
	for (Node* n : edgeBuffer[vertex1]) {
		if (n->vertex == vertex2) {
			if (facing == Facing::FRONT) {
				n->front = true;
			}
			else {
				n->back = true;
			}
			break;
		}
	}
}

// Initialize edge buffer data structure
void Renderable::initEdgeBuffer() {
	edgeBuffer = std::vector<std::list<Node*>>(verts.size() - 1);

	for (unsigned int i = 0; i < faces.size(); i += 3) {
		std::vector<unsigned int> face = std::vector<unsigned int>(3);
		face[0] = faces[i];
		face[1] = faces[i+1];
		face[2] = faces[i+2];
		std::sort(face.begin(), face.end());

		insertEdge(face[0], face[1]);
		insertEdge(face[0], face[2]);
		insertEdge(face[1], face[2]);
	}
}

// Sets front and back fields for whole edge buffer
void Renderable::populateEdgeBuffer(glm::vec3 eye) {
	for (unsigned int i = 0; i < faces.size(); i += 3) {
		std::vector<unsigned int> face = std::vector<unsigned int>(3);
		face[0] = faces[i];
		face[1] = faces[i+1];
		face[2] = faces[i+2];
		std::sort(face.begin(), face.end());

		unsigned int p1 = faces[i];
		unsigned int p2 = faces[i+1];
		unsigned int p3 = faces[i+2];

		glm::vec3 v1 = verts[p2] - verts[p1];
		glm::vec3 v2 = verts[p3] - verts[p1];

		glm::vec3 normal = glm::cross(v1, v2);
		float result = glm::dot(normal, eye-verts[p1]);
		if (result > 0) {
			updateEdge(face[0], face[1], Facing::FRONT);
			updateEdge(face[0], face[2], Facing::FRONT);
			updateEdge(face[1], face[2], Facing::FRONT);
		}
		else {
			updateEdge(face[0], face[1], Facing::BACK);
			updateEdge(face[0], face[2], Facing::BACK);
			updateEdge(face[1], face[2], Facing::BACK);
		}
	}
}

void Renderable::clearEdgeBuffer() {

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

// Prints edge buffer to console. For debugging and testing only
void Renderable::show() {
	int i = 0;
	for (std::list<Node*> l : edgeBuffer) {
		std::cout << "V" << i;
		for (Node* n : l) {
			std::cout << " -> " << n->vertex << ":" << n->front << ":" << n->back;
		}
		std::cout << std::endl;
		i++;
	}
}
