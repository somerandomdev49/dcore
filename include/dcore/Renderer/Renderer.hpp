#pragma once
#include <dcore/Uni.hpp>
#include <dcore/Renderer/RSkeletalMesh.hpp>
#include <dcore/Renderer/RStaticMesh.hpp>
#include <dcore/Renderer/RTexture.hpp>
#include <dcore/Renderer/RShader.hpp>
#include <dcore/Renderer/RModel.hpp>
#include <dcore/Renderer/RSkyBox.hpp>
#include <dcore/Renderer/RFastVertexBuffer.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Core/Span.hpp>
#include <dcore/Core/Type.hpp>

namespace dcore::platform
{
	class Context;
}

namespace dcore::graphics
{
	namespace details
	{
		struct Data;
	};
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
		 * @brief Renders a mesh.
		 * @note The shader must be bound!
		 * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
		 */
		void Render(RStaticMesh *mesh);

		/**
		 * @brief Renders a mesh without passing data to the shader.
		 * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
		 */
		void Render(RFastVertexBuffer *buf);

		/**
		 * @brief Renders a model.
		 * @note The shader must be bound!
		 * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
		 *
		 * @param textureUnit The texture unit bount when selecting textures.
		 */
		void Render(const Model *model, int textureUnit = 0);

		/**
		 * @brief Renders a skeletal mesh with a specified texture
		 * @warning Should be called only between OnBeginRender (private) and OnEndRender (private)
		 */
		void Render(RSkeletalMesh *mesh);

		enum DepthTestFunc
		{
			DepthTestFuncLess,
			DepthTestFuncLEqual
		};

		/**
		 * @brief Changes the depth test function
		 */
		void DepthTestFunction(DepthTestFunc func);

		/**
		 * @brief Enables depth testing.
		 */
		void EnableDepthCheck();

		/**
		 * @brief Disables depth testing.
		 */
		void DisableDepthCheck();

		void RenderToFramebuffer(bool set);
		void *GetFramebufferData();

		void SetWireframeMode(bool newIsWireframeMode);
		bool IsWireframeMode() const;

		void SetClearColor(const glm::vec4 &newColor) { ClearColor_ = newColor; }
		const glm::vec4 &GetClearColor() const { return ClearColor_; }

		const glm::vec2 &GetViewport() const { return ViewportSize_; }
		void SetViewport(glm::vec2 size);

		static Renderer *Instance();

	private:
		friend class dcore::platform::Context;
		friend class RenderResourceManager;

		static void SetInstance(Renderer *newInstance);

		void OnBeginRender();
		void OnEndRender();

		void Initialize();
		void DeInitialize();

		static void RModel_Constructor(const std::string &path, void *placement);
		static void RModel_DeConstructor(void *placement);

		static void RTexture_Constructor(const std::string &path, void *placement);
		static void RTexture_DeConstructor(void *placement);

		static void RStaticMesh_Constructor(const std::string &path, void *placement);
		static void RStaticMesh_DeConstructor(void *placement);

		static void RShader_Constructor(const std::string &path, void *placement);
		static void RShader_DeConstructor(void *placement);

		bool IsWireframeMode_  = false;
		bool ShouldRenderToFB_ = false;
		glm::vec4 ClearColor_;
		glm::vec2 ViewportSize_;

		struct details::Data *Data_;
	};

	/**
	 * @brief Contains vertices, indices and stride
	 */
	struct MeshData
	{
		std::vector<dstd::UInt32> Indices;
		std::vector<byte> VertexData;
		size_t Stride;
	};

	struct TextureIdSlot
	{
		std::string Name;
		std::string Id;
	};

	/**
	 * @brief Mesh data + texture index
	 */
	struct ModelMeshData
	{
		MeshData Mesh;
		dstd::USize TextureIndex;
	};

	/**
	 * @brief Contains the meshes and textures
	 */
	struct ModelData
	{
		std::vector<ModelMeshData> Meshes;
		std::vector<TextureIdSlot> Textures;
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
		 * Creates a static mesh from the provided indices, vertices and format (p - pos, n - norm, t - tex)
		 * */
		static void CreateStaticMesh(RStaticMesh *mesh, dstd::Span<const uint32_t> indices,
		                             dstd::Span<const byte> vertexData, const std::string &format = "pnt");

		/**
		 * Creates a static mesh from the provided indices, vertices and format (p - pos, n - norm, t - tex)
		 * */
		static void CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
		                             dstd::Span<const byte> vertexData, const std::string &format = "pnt");

		/**
		 * Creates a static mesh from the provided indices, vertices and format (p - pos, n - norm, t - tex)
		 * */
		static void CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
		                             const std::vector<byte> &vertexData, const std::string &format = "pnt");

		/**
		 * Creates a texture with the specified data, size and format.
		 * */
		static void CreateTexture(RTexture *texture, byte *data, const glm::ivec2 &size, TextureFormat format,
		                          TextureScaling scaling = TextureScaling::Linear, int unpackAlignment = -1);

		/**
		 * Creates a cubemap with the specified data for +x, -x, +y, -y, +z, -z faces, size and format.
		 * */
		static void CreateSkyBox(RSkyBox *skybox, glm::vec3 color, dstd::Span<byte *> datas, const glm::ivec2 &size,
		                         TextureFormat format, TextureScaling scaling = TextureScaling::Linear,
		                         int unpackAlignment = -1);

		/**
		 * Creates a fast vertex buffer with the specified index count.
		 * */
		static void CreateFastVertexBuffer(RFastVertexBuffer *buf, size_t indexCount);

		/**
		 * @brief Creates a model with the specified meshes and texture references.
		 *
		 * @param model (Output) the model to create.
		 * @param meshes The model meshes to include.
		 * @param textureSlots All the textures used in this model.
		 */
		static void CreateModel(Model *model, const ModelData &data);

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

		/**
		 * Deletes a skybox.
		 */
		static void DeleteSkyBox(RSkyBox *skybox);
	};
} // namespace dcore::graphics
