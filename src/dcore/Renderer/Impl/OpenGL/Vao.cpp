#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>

using namespace dcore::graphics::impl;

void opengl::Vao::Load(const std::vector<uint32_t> &indices, const std::vector<uint8_t> &vertexData, size_t stride)
{
	IndexCount_ = indices.size();

	glGenBuffers(1, &VBO_);
	glGenBuffers(1, &EBO_);
	glGenVertexArrays(1, &VAO_);
	glBindVertexArray(VAO_);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size(), &vertexData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

	LastOffset_ = 0;
	LastIndex_  = 0;
	Stride_     = stride;
}

void opengl::Vao::CreateFloatAttribute(int count)
{
	LastOffset_ += sizeof(float) * count;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Stride_, (void *)LastOffset_);
	glEnableVertexAttribArray(LastIndex_);
}

void opengl::Vao::Delete()
{
	glDeleteBuffers(1, &VBO_);
	glDeleteBuffers(1, &EBO_);
	glDeleteVertexArrays(1, &VAO_);
}
