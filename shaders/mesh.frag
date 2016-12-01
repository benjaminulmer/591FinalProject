#version 430 core

out vec4 colour;

uniform sampler2D attr;
uniform sampler2D image;
uniform int mode;
//uniform float R;

in vec3 N;
in vec3 L;
in vec3 V;
in vec2 UV;

void main(void) {    	
	// Diffuse
	//vec3 col = vec3(1.0, 1.0, 0.0);
    //colour = vec4(col * ((dot(N, L) +1) / 2), 1.0);

	vec4 colourImage;
	// Attribute-based texturing
	if (mode == 0) {
		// x axis for texture coordinate
		float diffuse =  (dot(N, L) + 1) / 2;

		// y axis for texture coordinate
		float R = 2.0;
		float tone = 1 - pow(abs(dot(normalize(N), normalize(V))), R);

		colourImage = texture(attr, vec2(diffuse, tone));
	}

	else if (mode == 1) { colourImage = texture(image, UV); }

    colour = colourImage;
}