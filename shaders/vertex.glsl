#version 450 core
//#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec4 offset;
//layout (location = 1) in vec4 color;

//out vec4 vs_color;
/*
out VS_OUT
{
	vec4 color; // Send color to the next stage
} vs_out;
*/

void main(void)
{
	const vec4 vertices[3] = vec4[3](	vec4( 0.25, -0.25,  0.5,  1.0),
										vec4(-0.25, -0.25,  0.5,  1.0),
										vec4( 0.25,  0.25,  0.5,  1.0)		);

	gl_Position = vertices[gl_VertexID] + offset;
	//vs_color = color;
	//vs_out.color = color; //vec4(1.0, 0.0, 0.0, 1.0);
}