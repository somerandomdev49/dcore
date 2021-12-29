#pragma once
#include <dcore/Uni.hpp>
#include <dcore/Renderer/RSkeletalMesh.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <dcore/Renderer/RFastVertexBuffer.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Uni.hpp>

namespace dcore::platform
{
	class Context;
}

namespace dcore::graphics
{
	/** Direct interface with OpenGL/DX/Vulkan/... */
	class Renderer
	{
	public:
		/**
		 * @brief Binds the shader
		 * @note All uniforms should be set after this call.
		 */
		void UseShader(RShader *shader);

		RUniform GetUniform(RShader *shader, const char *name);
		void SetUniform(const RUniform &u, float v);
		void SetUniform(const RUniform &u, int v);
		void SetUniform(const RUniform &u, const glm::vec2 &v);
		void SetUniform(const RUniform &u, const glm::vec3 &v);
		void SetUniform(const RUniform &u, const glm::vec4 &v);
		void SetUniform(const RUniform &u, const glm::mat2x2 &v);
		void SetUniform(const RUniform &u, const glm::mat2x3 &v);
		void SetUniform(const RUniform &u, const glm::mat2x4 &v);
		void SetUniform(const RUniform &u, const glm::mat3x2 &v);
		void SetUniform(const RUniform &u, const glm::mat3x3 &v);
		void SetUniform(const RUniform &u, const glm::mat3x4 &v);
		void SetUniform(const RUniform &u, const glm::mat4x2 &v);
		void SetUniform(const RUniform &u, const glm::mat4x3 &v);
		void SetUniform(const RUniform &u, const glm::mat4x4 &v);

		/**
		 * @brief Binds a texture unit. if texture is nullptr, nothing happens.
		 */
		void UseTexture(int unit, RTexture *texture);

		/**
		 * @brief Renders a mesh
		 * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
		 */
		void Render(RStaticMesh *mesh);

		/**
		 * @brief Renders a mesh without passing data to the shader.
		 * */
		void Render(RFastVertexBuffer *buf);

		/**
		 * @brief Renders a skeletal mesh with a specified texture
		 * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
		 */
		void Render(RSkeletalMesh *mesh);

		/**
		 * @brief Enables depth testing.
		 */
		void EnableDepthCheck();

		/**
		 * @brief Disables depth testing.
		 */
		void DisableDepthCheck();

		void SetWireframeMode(bool newIsWireframeMode);
		bool IsWireframeMode() const;

		static Renderer *Instance();

	private:
		friend class dcore::platform::Context;
		friend class RenderResourceManager;

		static void SetInstance(Renderer *newInstance);

		void OnBeginRender();
		void OnEndRender();

		void Initialize();
		void DeInitialize();

		static void RTexture_Constructor(const std::string &path, void *placement);
		static void RTexture_DeConstructor(void *placement);

		static void RStaticMesh_Constructor(const std::string &path, void *placement);
		static void RStaticMesh_DeConstructor(void *placement);

		static void RShader_Constructor(const std::string &path, void *placement);
		static void RShader_DeConstructor(void *placement);

		bool IsWireframeMode_ = false;
		glm::vec4 ClearColor_;
	};

	/** Class responsible for initializing/deinitializing render resources */
	class RenderResourceManager
	{
	public:
		static void Register(resource::ResourceLoader *rl);

		/** Default vertex type. */
		struct Vertex
		{
			glm::vec3 position, normal;
			glm::vec2 texcoord;
		};

		enum class TextureFormat
		{
			Red,
			Rg,
			Rgb,
			Rgba
		};

		enum class TextureScaling
		{
			Linear,
			Nearest
		};

		/**
		 * Creates a static mesh from the provided indices and vertices. (Wrapper around
		 * impl-specific stuff) If you can, use the byte version of this function. `vertices` will
		 * be unusable after this call. You should call `vertices.erase(vertices.begin(),
		 * vertices.end())` after this!
		 * */
		static void CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
		                             const std::vector<Vertex> &vertices);

		/**
		 * Creates a static mesh from the provided indices and vertices. (Wrapper around
		 * impl-specific stuff)
		 * */
		static void CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
		                             const std::vector<byte> &vertexData);

		/**
		 * Creates a texture with the specified data, size and format.
		 * */
		static void CreateTexture(RTexture *texture, byte *data, const glm::ivec2 &size, TextureFormat format,
		                          TextureScaling scaling = TextureScaling::Linear, int unpackAlignment = -1);

		/**
		 * Creates a fast vertex buffer with the specified index count.
		 * */
		static void CreateFastVertexBuffer(RFastVertexBuffer *buf, size_t indexCount);

		/**
		 * Deletes a fast vertex buffer.
		 * */
		static void DeleteFastVertexBuffer(RFastVertexBuffer *buf);

		/**
		 * Deletes a static mesh.
		 * */
		static void DeleteStaticMesh(RStaticMesh *mesh);

		/**
		 * Deletes a texture.
		 */
		static void DeleteTexture(RTexture *texture);
	};
} // namespace dcore::graphics
