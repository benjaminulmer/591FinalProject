#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <cstring>
#include <cstdio>

#include "Renderable.h"

struct PackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

class ContentLoading {

public:
	static Renderable* createRenderable(std::string modelFile);

private:
	static bool getSimilarVertexIndex_fast(
		PackedVertex & packed, 
		std::map<PackedVertex,unsigned short> & VertexToOutIndex,
		unsigned short & result);
	static void indexVBO(
		std::vector<glm::vec3> & in_vertices,
		std::vector<glm::vec2> & in_uvs,
		std::vector<glm::vec3> & in_normals,
		std::vector<unsigned short> & out_indices,
		std::vector<glm::vec3> & out_vertices,
		std::vector<glm::vec2> & out_uvs,
		std::vector<glm::vec3> & out_normals);
	static bool loadOBJ(
		const char * path, 
		std::vector<glm::vec3> & out_vertices, 
		std::vector<glm::vec2> & out_uvs,
		std::vector<glm::vec3> & out_normals,
		std::vector<GLushort> &out_faces,
		std::vector<glm::vec3> & raw_verts);
};

