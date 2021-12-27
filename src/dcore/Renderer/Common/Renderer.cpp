#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>

namespace dcore::graphics
{

	static Renderer *rendererInst = nullptr;

	Renderer *Renderer::Instance()
	{
		if(!rendererInst) rendererInst = new Renderer;
		return rendererInst;
	}

	void Renderer::SetInstance(Renderer *newInstance) { rendererInst = newInstance; }

	bool Renderer::IsWireframeMode() const { return IsWireframeMode_; }

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
		delete mesh;
	}

	void RenderResourceManager::Register(resource::ResourceLoader *rl)
	{
		rl->RegisterResourceType<RTexture>("Texture");
		resource::ResourceManager::Instance()->RegisterConstructor<RTexture>(
		    &Renderer::RTexture_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RTexture>(
		    &Renderer::RTexture_DeConstructor);

		rl->RegisterResourceType<RShader>("Shader");
		resource::ResourceManager::Instance()->RegisterConstructor<RShader>(
		    &Renderer::RShader_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RShader>(
		    &Renderer::RShader_DeConstructor);

		rl->RegisterResourceType<RStaticMesh>("Mesh");
		resource::ResourceManager::Instance()->RegisterConstructor<RStaticMesh>(
		    &Renderer::RStaticMesh_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RStaticMesh>(
		    &Renderer::RStaticMesh_DeConstructor);
	}

	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh,
	                                             const std::vector<uint32_t> &indices,
	                                             const std::vector<Vertex> &vertices)
	{
		if(!mesh) return;
		std::vector<byte> vertexData;

		// Converting from std::vector<Vertex> to std::vector<byte> (`vertices` must be unusable
		// now, somehow this works with a const vector...) I hope this doesn't allocate another
		// vector of the same size or atleast deallocs vertices afterward.
		vertexData.insert(vertexData.end(), std::make_move_iterator((byte *)&vertices[0]),
		                  std::make_move_iterator((byte *)&vertices[vertices.size()]));

		CreateStaticMesh(mesh, indices, vertexData);
	}
} // namespace dcore::graphics
