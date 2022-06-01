#pragma once
#include <dcore/Renderer/Impl/OpenGL/GL.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Uni.hpp>
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
		void Load(const std::vector<dstd::UInt32> &indices, const std::vector<byte> &vertexData, dstd::USize stride);
		void Load(const std::vector<dstd::UInt32> &indices, const byte *vertexData, dstd::USize vertexDataSize, dstd::USize stride);
		void Load(const dstd::UInt32 *indices, dstd::USize indicesSize, const std::vector<byte> &vertexData, dstd::USize stride);
		void Load(const dstd::UInt32 *indices, dstd::USize indicesSize, const byte *vertexData, dstd::USize vertexDataSize,
		          dstd::USize stride);
		void CreateFloatAttribute(int count);
		void CreateIntAttribute(int count);
		void Delete();
	};
} // namespace dcore::graphics::impl::opengl
