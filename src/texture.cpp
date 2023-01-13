#include "texture.h"
#include <glad/glad.h>
#include <stb_image/stb_image.h>

Texture::Texture(const std::string& path)
	: m_renderer_id(0), m_filepath(path), m_local_buffer(nullptr), m_width(0), m_height(0), m_bpp(0)
{
	// depends on texture format, do flip for png
	stbi_set_flip_vertically_on_load(true);
	m_local_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	glGenTextures(1, &m_renderer_id);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);

	// always supply these 4 parameters, else black screen
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR_MIPMAP_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // try GL_CLAMP_TO_BORDER, GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // try GL_CLAMP_TO_BORDER, GL_REPEAT

	//glGenerateMipmap(GL_TEXTURE_2D);
	// modern opengl: GL_RGBA8
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	// one can keep local data around for something like sampling, ...
	if (m_local_buffer)
		stbi_image_free(m_local_buffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_renderer_id);
}

void Texture::Bind(unsigned int slot) const
{
	// query to find out how many texture slots, if you use many
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
