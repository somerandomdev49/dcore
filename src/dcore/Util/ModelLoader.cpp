#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Data/FileInput.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Uni.hpp>

#include <vector>
#include <string>

namespace dcore::util
{
	namespace
	{
		struct BufferFile
		{
			dstd::USize Size;
			std::string Path;
		};
	} // namespace

	bool LoaderUtil::LoadModel(ModelData &d, const std::string &folder, const std::string &gltfPath)
	{
		data::FileInput fi(folder, gltfPath);
		data::adapters::JsonInputAdapter jia;
		fi.SetAdapter(&jia);
		fi.Read();
		auto &gltf = fi.Get();

		std::vector<BufferFile> bufferFiles;
		bufferFiles.reserve(gltf["buffers"].size());

		for(const auto &buffer : gltf["buffers"])
		{
			bufferFiles.push_back(
			    BufferFile {buffer["byteLength"].get<dstd::USize>(), folder + "/" + buffer["uri"].get<std::string>()});
		}

		// Overall, we ignore scenes, nodes and meshes and only include "primitives" in output.
	}
} // namespace dcore::util
