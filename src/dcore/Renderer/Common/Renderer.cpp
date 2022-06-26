#include "dcore/Core/Assert.hpp"
#include "dcore/Renderer/RSkyBox.hpp"
#include <_ctype.h>
#include <algorithm>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Util/LoaderUtil.hpp>
#include <dcore/Renderer/RModel.hpp>
#include <dcore/Core/Log.hpp>
#include <string>

namespace dcore::graphics
{

	static Renderer *rendererInst = nullptr;

	Renderer *Renderer::Instance()
	{
		if(rendererInst == nullptr) rendererInst = new Renderer;
		return rendererInst;
	}

	void Renderer::SetInstance(Renderer *newInstance) { rendererInst = newInstance; }

	bool Renderer::IsWireframeMode() const { return IsWireframeMode_; }

	void Renderer::Render(const Model *model, int textureUnit /* = 0 */)
	{
		// fprintf(stderr, "SHEEEEEEEEEEEEEEEEEEEEEESH\n");
		for(const auto &mesh : model->Meshes_)
		{
			UseTexture(textureUnit, model->TextureSlots_[mesh.TextureIndex].Texture);
			Render(mesh.Mesh);
		}
		// fprintf(stderr, "YO MAMA\n");
	}

	void Renderer::RStaticMesh_Constructor(const std::string &path, void *placement)
	{
		auto *mesh = new(placement) RStaticMesh();
		MeshData data;
		std::string format = "pnt";
		if(auto loc = path.find_last_of(":"); loc != std::string::npos)
			format = path.substr(loc + 1);
		util::LoaderUtil::LoadMesh(data, path.substr(0, path.find_last_of(":")), format);
		RenderResourceManager::CreateStaticMesh(mesh, data.Indices, data.VertexData, format);
	}

	void Renderer::RStaticMesh_DeConstructor(void *placement)
	{
		auto *mesh = reinterpret_cast<RStaticMesh *>(placement);
		RenderResourceManager::DeleteStaticMesh(mesh);
		delete mesh;
	}

	void Renderer::RModel_Constructor(const std::string &path, void *placement)
	{
		auto *model = new(placement) Model();
		ModelData data;

		std::string modelName = path.substr(path.find_last_of('/') + 1);
		std::string gltf  = modelName + ".gltf";

		util::LoaderUtil::LoadModel(data, path, gltf);
		RenderResourceManager::CreateModel(model, data);
	}

	void Renderer::RModel_DeConstructor(void *placement)
	{
		auto *model = reinterpret_cast<Model *>(placement);
		delete model;
	}

	void Renderer::RTexture_Constructor(const std::string &path, void *placement)
	{
		using RRM = RenderResourceManager;

		static RRM::TextureFormat formats[4] = {RRM::TextureFormat::Red, RRM::TextureFormat::Rg,
		                                        RRM::TextureFormat::Rgb, RRM::TextureFormat::Rgba};

		// This constructs a RTexture at the specified address (see "placement new")
		auto *tex = new(placement) RTexture();

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
		auto *tex = reinterpret_cast<RTexture *>(placement);
		RenderResourceManager::DeleteTexture(tex);
		delete tex;
	}

	void Renderer::RenderToFramebuffer(bool set)
	{
		ShouldRenderToFB_ = set;
	}

	namespace
	{
		static inline bool ReplaceAll(std::string &str, const std::string &target, const std::string &replacement)
		{
			if(target.empty()) return false;

			size_t start_pos = 0;
			const bool found_substring = str.find(target, start_pos) != std::string::npos;

			while((start_pos = str.find(target, start_pos)) != std::string::npos)
			{
				str.replace(start_pos, target.length(), replacement);
				start_pos += replacement.length();
			}

			return found_substring;
		}

		static inline std::string ToLower(std::string &&str)
		{
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			return std::move(str);
		}

		void RSkyBox_Constructor(const std::string &path, void *placement)
		{
			using RRM = RenderResourceManager;

			static RRM::TextureFormat formats[4] = {RRM::TextureFormat::Red, RRM::TextureFormat::Rg,
													RRM::TextureFormat::Rgb, RRM::TextureFormat::Rgba};

			const auto loadTexture = [&](const std::string &path, byte *&data, glm::ivec2 &size, RRM::TextureFormat &fmt)
			{
				util::ImageData d;
				util::LoaderUtil::LoadImage(d, path);
				if(d.data == nullptr)
				{
					DCORE_LOG_ERROR << "Failed to load skybox texture at '" << path << "'";
					return;
				}
				fmt = formats[d.channels - 1];
				size = d.size;
				// DCORE_ASSERT(myFmt != fmt && d.size == size, "Bad skybox image");
				data = (byte*)d.data;
			};

			const auto getFormattedPath = [&](int negative, int direction) -> std::string
			{
				static const char *S[] = { "P", "N" };
				static const char *Z[] = { "Pos", "Neg" };
				static const char *W[] = { "Positive", "Negative" };
				static const char *C[] = { "+", "-" };
				static const char *D[] = { "X", "Y", "Z" };
				std::string p = path;
				ReplaceAll(p, ":S", S[negative]);
				ReplaceAll(p, ":Z", Z[negative]);
				ReplaceAll(p, ":W", W[negative]);
				ReplaceAll(p, ":C", C[negative]);
				ReplaceAll(p, ":D", D[direction]);
				ReplaceAll(p, ":s", ToLower(S[negative]));
				ReplaceAll(p, ":z", ToLower(Z[negative]));
				ReplaceAll(p, ":w", ToLower(W[negative]));
				ReplaceAll(p, ":c", ToLower(C[negative]));
				ReplaceAll(p, ":d", ToLower(D[direction]));
				return p;
			};

			byte *datas[6] = {0};
			
			glm::ivec2 size;
			RRM::TextureFormat fmt;

			for(int i = 0; i < 3; ++i) {
				loadTexture(getFormattedPath(0, i), datas[i * 2 + 0], size, fmt);
				loadTexture(getFormattedPath(1, i), datas[i * 2 + 1], size, fmt);
			}
			auto *box = new(placement) RSkyBox();
			RenderResourceManager::CreateSkyBox(box, { datas, 6 }, size, fmt);
			for(int i = 0; i < 6; ++i) free(datas[i]);
		}

		void RSkyBox_DeConstructor(void *placement)
		{
			auto *tex = reinterpret_cast<RSkyBox *>(placement);
			RenderResourceManager::DeleteSkyBox(tex);
			delete tex;
		}
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

		rl->RegisterResourceType<RSkyBox>("SkyBox");
		resource::ResourceManager::Instance()->RegisterConstructor<RSkyBox>(&RSkyBox_Constructor);
		resource::ResourceManager::Instance()->RegisterDeConstructor<RSkyBox>(&RSkyBox_DeConstructor);
	}

	void RenderResourceManager::CreateStaticMesh(RStaticMesh *mesh, const std::vector<uint32_t> &indices,
	                                             const std::vector<Vertex> &vertices)
	{
		if(mesh == nullptr) return;
		std::vector<byte> vertexData;

		// Converting from std::vector<Vertex> to std::vector<byte> (`vertices` must be unusable
		// now, somehow this works with a const vector...) I hope this doesn't allocate another
		// vector of the same size or atleast deallocs vertices afterward.
		vertexData.insert(vertexData.end(), std::make_move_iterator((byte *)vertices.data()),
		                  std::make_move_iterator((byte *)&vertices[vertices.size()]));

		CreateStaticMesh(mesh, indices, vertexData);
	}

	void RenderResourceManager::CreateModel(Model *model, const ModelData &data)
	{
		for(const auto &mesh : data.Meshes)
		{
			auto *sm = new RStaticMesh();
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
