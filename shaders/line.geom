#version 430 core

uniform mat4 view;
uniform mat4 projection;

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

void main(void) {
	vec3 line = (gl_in[0].gl_Position - gl_in[1].gl_Position).xyz;
	vec3 V = (view * gl_in[0].gl_Position).xyz;
	vec3 tan = normalize(cross(line, V));

	gl_Position = projection * view * (gl_in[0].gl_Position - 0.01*vec4(tan, 0.0));
	EmitVertex();

	gl_Position = projection * view * (gl_in[1].gl_Position - 0.01*vec4(tan, 0.0));
	EmitVertex();

	gl_Position = projection * view * (gl_in[0].gl_Position + 0.01*vec4(tan, 0.0));
	EmitVertex();

	gl_Position = projection * view * (gl_in[1].gl_Position + 0.01*vec4(tan, 0.0));
	EmitVertex();
	
	//for(int i = 0; i < 2; i++) {
	//	gl_Position = projection * view * gl_in[i].gl_Position;
	//	EmitVertex();
	//}
	EndPrimitive();
}