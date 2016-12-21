#version 430 core

out vec4 colour;

uniform sampler2D attr;
uniform sampler2D mult;
uniform sampler2D image;
uniform int mode;
uniform int multiply;
uniform int attrMode;
uniform float R;

in vec3 N;
in vec3 L;
in vec3 V;
in vec2 UV;
in float Z;

void main(void) {    	

	vec4 colourImage;
	vec4 attrColour;

	/***** Attribute-based texturing *****/
	// x axis for texture coordinate
	float diffuse =  (dot(N, L) + 1) / 2;

	// Orientation-based attribute mapping
	if (attrMode == 0) {
		// y axis for texture coordinate
		float tone = 1 - pow(abs(dot(normalize(N), normalize(V))), R);
	
		attrColour = texture(attr, vec2(diffuse, tone));
	}
	
	// Depth-based attribute mapping
	else {
		float r = 30;
		float zmin = 3.0;
		float zmax = r * zmin;
		float D = max((log(Z/zmin) / log(zmax/zmin)), 0);
		attrColour = texture(attr, vec2(diffuse, D));
	} 

	/***** Image-based texturing *****/
	vec4 imgColour = texture(image, UV);

	if (mode == 0) {
		if (multiply > 0) { colour = attrColour * imgColour; }
		else { colour = (attrColour * 0.5) + (imgColour * 0.5); }
	}
	else if (mode == 1) colour = attrColour;
	else if (mode == 2) colour = imgColour;
}