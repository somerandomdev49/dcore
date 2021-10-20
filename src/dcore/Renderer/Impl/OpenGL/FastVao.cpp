#include <dcore/Renderer/Impl/OpenGL/FastVao.hpp>
#include <dcore/Uni.hpp>
#include <cstdio>

using namespace dcore::graphics::impl;

void opengl::FastVao::Load(size_t indexCount)
{
	IndexCount_ = indexCount;

	VBO_ = Gl::GenerateBuffer();
	VAO_ = Gl::GenerateVertexArray();
	Gl::BindVertexArray(VAO_);
	Gl::BindBuffer(ArrayBuffer, VBO_);
	byte *tmpData = new byte[indexCount];
	Gl::SetBufferData(ArrayBuffer, indexCount, tmpData, BufferUsageStaticDraw);
	delete tmpData;
}

void opengl::FastVao::Delete()
{
	Gl::DeleteBuffer(VBO_);
	Gl::DeleteVertexArray(VAO_);
}
