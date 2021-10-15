#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>
using namespace dcore::graphics;

static Renderer *rendererInst = nullptr;

Renderer *Renderer::Instance()
{
	if(!rendererInst) rendererInst = new Renderer;
	return rendererInst;
}

void Renderer::SetInstance(Renderer *newInstance) { rendererInst = newInstance; }

void Renderer::RStaticMesh_Constructor(const std::string &path, void *placement)
{
	RStaticMesh *mesh = new(placement) RStaticMesh();
	util::MeshData d;
	util::LoaderUtil::LoadMesh(d, path, "pnt");
	RenderResourceManager::CreateStaticMesh(mesh, d.indices, d.vertexData);
}

void Renderer::RStaticMesh_DeConstructor(void *placement)
{
	RStaticMesh *mesh = reinterpret_cast<RStaticMesh *>(placement);
	RenderResourceManager::DeleteStaticMesh(mesh);
}
