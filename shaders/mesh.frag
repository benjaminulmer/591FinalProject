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

	vec4 colourImage;

	/***** Attribute-based texturing *****/
	// x axis for texture coordinate
	float diffuse =  (dot(N, L) + 1) / 2;

	// y axis for texture coordinate
	float R = 2.0;
	float tone = 1 - pow(abs(dot(normalize(N), normalize(V))), R);

	vec4 attrColour = texture(attr, vec2(diffuse, tone));

	/***** Image-based texturing *****/
	vec4 imgColour = texture(image, UV);

	if (mode == 0) colour = (attrColour * 0.5) + (imgColour * 0.5);
	else if (mode == 1) colour = attrColour;
	else if (mode == 2) colour = imgColour;
}