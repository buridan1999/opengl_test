#version 450 core
//#extension GL_ARB_separate_shader_objects : enable

/*
in VS_OUT
{
	vec4 color; // Send color to the next stage
} vs_in;
*/
//in vec4 vs_color;

out vec4 color;

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0); //vs_in.color; //vec4(1.0, 0.0, 0.0, 1.0); // fs_in.color;
}