#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Renderer/RModel.hpp>
#include <dcore/Core/Log.hpp>

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

	void Renderer::Render(const Model *model, int textureUnit /* = 0 */)
	{
		for(const auto &mesh : model->Meshes_)
		{
			UseTexture(textureUnit, model->TextureSlots_[mesh.TextureIndex].Texture);
			Render(mesh.Mesh);
		}
	}

	void Renderer::RStaticMesh_Constructor(const std::string &path, void *placement)
	{
		RStaticMesh *mesh = new(placement) RStaticMesh();
		MeshData d;

		RenderResourceManager::CreateStaticMesh(mesh, d.Indices, d.VertexData);
	}

	void Renderer::RStaticMesh_DeConstructor(void *placement)
	{
		RStaticMesh *mesh = reinterpret_cast<RStaticMesh *>(placement);
		RenderResourceManager::DeleteStaticMesh(mesh);
		delete mesh;
	}

	void Renderer::RModel_Constructor(const std::string &path, void *placement)
	{
		Model *model = new(placement) Model();
		ModelData data;

		std::string modelName = path.substr(path.find_last_of('/') + 1);
		std::string gltfFile  = modelName + ".gltf";

		util::LoaderUtil::LoadModel(data, path, gltfFile);
		RenderResourceManager::CreateModel(model, data);
	}

	void Renderer::RModel_DeConstructor(void *placement)
	{
		Model *model = reinterpret_cast<Model *>(placement);
		delete model;
	}

	void Renderer::RTexture_Constructor(const std::string &path, void *placement)
	{
		using RRM = RenderResourceManager;

		static RRM::TextureFormat formats[4] = {RRM::TextureFormat::Red, RRM::TextureFormat::Rg,
		                                        RRM::TextureFormat::Rgb, RRM::TextureFormat::Rgba};

		// This constructs a RTexture at the specified address (see "placement new")
		RTexture *tex = new(placement) RTexture();

		util::ImageData d;
		// DCORE_LOG_INFO << "loading image from " << path;
		util::LoaderUtil::LoadImage(d, path);
		if(d.data == nullptr)
		{
			DCORE_LOG_ERROR << "Failed to load texture at '" << path << "'";
			return;
		}
		auto fmt = formats[d.channels - 1];

		// DCORE_LOG_INFO << "Generating image (" << d.channels << 'x' << d.size.x << 'x' << d.size.y << ')';
		RenderResourceManager::CreateTexture(tex, (byte *)d.data, d.size, fmt);

		free(d.data);
	}

	void Renderer::RTexture_DeConstructor(void *placement)
	{
		RTexture *tex = reinterpret_cast<RTexture *>(placement);
		RenderResourceManager::DeleteTexture(tex);
		delete tex;
	}

	void RenderResourceManager::Register(resource::ResourceLoader *rl)
	{
		rl->RegisterResourceType<RTexture>("Texture");
		resource::ResourceManager::Instance()->RegisterConstructor<RTexture>(&Renderer::RTexture_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RTexture>(&Renderer::RTexture_DeConstructor);

		rl->RegisterResourceType<RShader>("Shader");
		resource::ResourceManager::Instance()->RegisterConstructor<RShader>(&Renderer::RShader_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RShader>(&Renderer::RShader_DeConstructor);

		rl->RegisterResourceType<RStaticMesh>("Mesh");
		resource::ResourceManager::Instance()->RegisterConstructor<RStaticMesh>(&Renderer::RStaticMesh_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RStaticMesh>(&Renderer::RStaticMesh_DeConstructor);

		rl->RegisterResourceType<Model>("Model");
		resource::ResourceManager::Instance()->RegisterConstructor<Model>(&Renderer::RModel_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<Model>(&Renderer::RModel_DeConstructor);
	}

	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
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

	void RenderResourceManager::CreateModel(Model *model, const ModelData &data)
	{
		for(const auto &mesh : data.Meshes)
		{
			RStaticMesh *sm = new RStaticMesh();
			CreateStaticMesh(sm, mesh.Mesh.Indices, mesh.Mesh.VertexData);
			model->Meshes_.push_back(ModelMesh {sm, mesh.TextureIndex});
		}

		for(const auto &texture : data.Textures)
		{
			model->TextureSlots_.push_back(
			    TextureSlot {texture.Name, resource::GetResource<RTexture>(texture.Id).Get()});
		}
	}

} // namespace dcore::graphics
