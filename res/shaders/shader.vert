#version 450 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_tex_coord;

out vec2 tex_coord;

// location has to be 2 because of the other specified locations
layout(location = 2) uniform mat4 u_mvp; // model view projection matrix

void main()
{
	gl_Position = u_mvp * a_position;
	tex_coord = a_tex_coord;
}