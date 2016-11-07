#version 430 core

out vec4 colour;

in vec3 N;
in vec3 L;
in vec3 V;

void main(void) {    	
	// Diffuse
	vec3 col = vec3(0.0, 1.0, 0.0);
    colour = vec4(col * ((dot(N, L) +1) / 2), 1.0);
}
