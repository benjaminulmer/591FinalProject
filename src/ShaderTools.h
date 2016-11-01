#pragma once

#include <GL/glew.h>
#include <iostream>
#include <fstream>

class ShaderTools {

public:
	static unsigned long getFileLength(std::ifstream& file);
	static GLchar* loadshader(std::string filename);
	static void unloadshader( GLchar** ShaderSource );
	static GLuint compileShaders(const char* vertexFilename, const char* fragmentFilename);
};
