#version 450 core

in vec2 tex_coord;

out vec4 color;

layout(location = 0) uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, tex_coord);
}