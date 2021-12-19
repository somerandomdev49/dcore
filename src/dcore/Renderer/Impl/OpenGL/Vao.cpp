#include <dcore/Renderer/Impl/OpenGL/Vao.hpp>
#include <dcore/Uni.hpp>
#include <cstdio>

namespace dcore::graphics::impl
{
	void opengl::Vao::Load(const std::vector<uint32_t> &indices, const std::vector<byte> &vertexData, size_t stride)
	{
		IndexCount_ = indices.size();

		VBO_ = Gl::GenerateBuffer();
		EBO_ = Gl::GenerateBuffer();
		VAO_ = Gl::GenerateVertexArray();
		Gl::BindVertexArray(VAO_);
		Gl::BindBuffer(ArrayBuffer, VBO_);
		Gl::SetBufferData(ArrayBuffer, vertexData.size(), &vertexData[0], BufferUsageStaticDraw);

		Gl::BindBuffer(ElementArrayBuffer, EBO_);
		Gl::SetBufferData(ElementArrayBuffer, indices.size() * sizeof(uint32_t), &indices[0], BufferUsageStaticDraw);

		LastOffset_ = 0;
		LastIndex_  = 0;
		Stride_     = stride;
	}

	void opengl::Vao::CreateFloatAttribute(int count)
	{
		// printf("CreateFloatAttribute(count = %d): LastIndex_ = %zu, LastOffset_ = %zu, Stride_ = %zu\n", count,
		// LastIndex_,
		//        LastOffset_, Stride_);
		Gl::VertexAttributePointer(LastIndex_, count, TypeFloat, Stride_, LastOffset_);
		Gl::EnableVertexAttributeArray(LastIndex_);
		LastOffset_ += sizeof(float) * count;
		LastIndex_ += 1;
	}

	void opengl::Vao::Delete()
	{
		Gl::DeleteBuffer(VBO_);
		Gl::DeleteBuffer(EBO_);
		Gl::DeleteVertexArray(VAO_);
	}
} // namespace dcore::graphics::impl
