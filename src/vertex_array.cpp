#include "vertex_array.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_renderer_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_renderer_id);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * VertexBufferElement::get_size_of_type(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_renderer_id);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
