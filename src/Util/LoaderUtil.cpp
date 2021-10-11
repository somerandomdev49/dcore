#include <dcore/Util/LoaderUtil.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace dcore::util;

ImageData LoaderUtil::LoadImage(const std::string &path)
{
	ImageData d;
	d.data = stbi_load(path.c_str(), &d.size.x, &d.size.y, &d.channels, 0);
	return d;
}

MeshData LoaderUtil::LoadMesh(const std::string &path)
{
	MeshData d;
	
}
