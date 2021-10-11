#pragma once
#include <dcore/Renderer/Impl/OpenGL.hpp>
#include <cinttypes>
#include <vector>

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::graphics::impl::opengl
{
	class Vao
	{
		struct Attribute
		{
			enum
			{
				Float,
				Int
			} Type;
			int Count;
			int Offset;
		};

		UInt VBO_, VAO_, EBO_;
		size_t IndexCount_;

		size_t Stride_, LastOffset_, LastIndex_;
		std::vector<Attribute> Attributes_;

		friend class dcore::graphics::Renderer;

	public:
		void Load(const std::vector<uint32_t> &indices, const std::vector<uint8_t> &verticexData, size_t stride);
		void CreateFloatAttribute(int count);
		void CreateIntAttribute(int count);
		void Delete();
	};
} // namespace dcore::graphics::impl::opengl
