#pragma once
#include <dcore/Renderer/RSkeletalMesh.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>

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
		 * @brief Renders a skeletal mesh with a specified texture
		 * @note if shader is nullptr, the previous shader is used.
		 * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
		 */
		void Render(RShader *shader, RSkeletalMesh *mesh, RTexture *texture);

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

		/**
		 * Creates a static mesh from the provided indices and vertices. (Wrapper around impl-specific stuff)
		 * If you can, use the uint8_t version of this function.
		 * `vertices` will be unusable after this call.
		 * You should call `vertices.erase(vertices.begin(), vertices.end())` after this!
		 * */
		static void CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices, const std::vector<Vertex> &vertices);

		/**
		 * Creates a static mesh from the provided indices and vertices. (Wrapper around impl-specific stuff)
		 * */
		static void CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices, const std::vector<uint8_t> &vertexData);

		/** Deletes a static mesh from the provided indices and vertices. (Wrapper around impl-specific stuff) */
		static void DeleteStaticMesh(RStaticMesh *mesh);
	};
} // namespace dcore::graphics
