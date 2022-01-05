#pragma once
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Uni.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace dcore::util
{
	struct ImageData
	{
		void *data;
		int channels;
		glm::ivec2 size;
	};

	// struct MeshData
	// {
	// 	std::vector<dstd::UInt32> indices;
	// 	std::vector<byte> vertexData;
	// 	size_t stride;
	// };

	// struct ModelData
	// {
	// 	/** pair<Name, Path> */
	// 	std::vector<std::pair<std::string, std::string>> Textures;
	// 	/** pair<Name, Path> */
	// 	std::vector<MeshData> Meshes;
	// };

	class LoaderUtil
	{
	public:
		/** Loads an image. */
		static bool LoadImage(ImageData &d, const std::string &path);

		/** Loads a mesh using the format for storing vertices.
		 * (p - position, n - normal, t - texcoords) */
		static bool LoadMesh(graphics::MeshData &d, const std::string &path, const std::string &format = "pnt");

		/** Loads a file into a string. */
		static bool LoadFile(std::string &out, const std::string &path);

		/**
		 * @brief Loads a model from a folder.
		 *
		 * The folder must contain a .gltf and a .bin file.
		 *
		 * @param d (Output) Data.
		 * @param folder Absolute path to the folder which contains the .gltf and the .bin file.
		 * @param gltfPath Relative (to the @c folder) path to the .gltf file.
		 * @return true if the loading was successfull, false otherwise.
		 */
		static bool LoadModel(graphics::ModelData &d, const std::string &folder, const std::string &gltfPath);
	};
} // namespace dcore::util
