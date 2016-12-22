#version 430 core

out vec4 colour;

uniform sampler2D attr;
uniform sampler2D image;
uniform int texMode;
uniform int attrMode;
uniform float R;

in vec3 N;
in vec3 L;
in vec3 V;
in vec2 UV;
in float Z;

void main(void) {    	
;
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

	if (texMode == 0) {
		colour = attrColour * imgColour;
	}
	else if (texMode == 1) {
		colour = (attrColour * 0.5) + (imgColour * 0.5);
	}
	else if (texMode == 2) {
		colour = attrColour;
	}
	else if (texMode == 3) {
		colour = imgColour;
	}
}