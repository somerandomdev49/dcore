#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <cinttypes>
#include <vector>

namespace dcore::graphics::opengl
{
	using Index = std::uint32_t;
	struct Vertex
	{
		
	};

	class Vao
	{
		UInt VBO, VAO, EBO;
	public:
		void Create(const std::vector<Index> &indices,
		            const std::vector<Vertex> &vertices);

		void Bind();
		// void CreateIndexBuffer   (UInt id, void *data);
		// void CreateVertexBuffer  (UInt id, void *data);
		void CreateFloatAttribute(UInt id, void *data, int count);
		void CreateIntAttribute  (UInt id, void *data, int count);
		void UnBind();
	};
}
