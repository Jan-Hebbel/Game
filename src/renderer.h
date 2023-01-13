#pragma once

#include "vertex_array.h"
#include "index_buffer.h"
#include "shader.h"

class Renderer
{
public:
	void Clear() const;
	// shader is abnormal, normally you would pass a material (shader and its uniforms)
	// this way you also get rid of shader uniform calls in the renderer loop in the main file
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
private:
};