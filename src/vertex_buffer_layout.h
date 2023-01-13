#pragma once
#include <glad/glad.h>
#include <vector>
#include "error.h"

struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int get_size_of_type(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:         return sizeof(float); // 4
			case GL_UNSIGNED_INT:  return sizeof(unsigned int); // 4
			case GL_UNSIGNED_BYTE: return sizeof(unsigned char); // 1
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() 
		: m_stride(0) {}

	void Push(unsigned int type, unsigned int count)
	{
		bool normalized = GL_FALSE;
		if (type == GL_UNSIGNED_BYTE)
			normalized = GL_TRUE;
		m_elements.push_back({ type, count, normalized });
		m_stride += VertexBufferElement::get_size_of_type(type) * count; // thecherno did this with "* count"?!
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride;  }

private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};