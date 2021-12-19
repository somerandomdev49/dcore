#include <dcore/Renderer/Impl/OpenGL/FastVao.hpp>
#include <dcore/Uni.hpp>
#include <cstdio>

namespace dcore::graphics::impl
{

	void opengl::FastVao::Load(size_t indexCount)
	{
		IndexCount_ = indexCount;
		VAO_        = Gl::GenerateVertexArray();
		Gl::BindVertexArray(VAO_);
	}

	void opengl::FastVao::Delete() { Gl::DeleteVertexArray(VAO_); }
} // namespace dcore::graphics::impl
