#pragma once
#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <dcore/Uni.hpp>
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
		// struct Attribute
		// {
		// 	enum
		// 	{
		// 		Float,
		// 		Int
		// 	} Type;
		// 	int Count;
		// 	int Offset;
		// };

		UInt VBO_, VAO_, EBO_;
		size_t IndexCount_;

		size_t Stride_, LastOffset_, LastIndex_;
		// std::vector<Attribute> Attributes_;

		friend class dcore::graphics::Renderer;

	public:
		void Load(const std::vector<uint32_t> &indices, const std::vector<byte> &vertexData,
		          size_t stride);
		void Load(const std::vector<uint32_t> &indices, const byte *vertexData,
		          size_t vertexDataSize, size_t stride);
		void Load(const uint32_t *indices, size_t indicesSize, const std::vector<byte> &vertexData,
		          size_t stride);
		void Load(const uint32_t *indices, size_t indicesSize, const byte *vertexData,
		          size_t vertexDataSize, size_t stride);
		void CreateFloatAttribute(int count);
		void CreateIntAttribute(int count);
		void Delete();
	};
} // namespace dcore::graphics::impl::opengl
