#include <dg/Loaders/MainWorldLoader.hpp>

#if 0 // Future API
#include <dg/Networking/ServerInterface.hpp>
#endif

#include <dcore/Platform/Platform.hpp>
#include <dcore/Data/FileInput.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Util/JsonConverters.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dcore/Core/Log.hpp>

#include <dg/Entity/CharacterController.hpp>
#include <dg/Entity/CameraFollow.hpp>

#include <vector>
#include <imgui.h>

#define COMMAND_BUFFER_LENGTH 128
namespace
{
	char *AllocEmptyCString(dstd::USize size)
	{
		char *str = new char[size];
		std::fill(str, str + size, 0);
		return str;
	}

	class BaseDebugLayer : public dcore::world::DebugLayer
	{
		bool ShowImGuiDemoWindow_ = false;
	public:
		void OnRender(dcore::graphics::RendererInterface *renderer) override
		{
			ImGui::Begin("[DEBUG] Screen Controls");
			ImGui::Checkbox("Show Demo Window", &ShowImGuiDemoWindow_);
			ImGui::End();
			
			if(ShowImGuiDemoWindow_) ImGui::ShowDemoWindow();
		}
	};

	class ConsoleDebugLayer : public dcore::world::DebugLayer
	{
		char *CommandBuffer_;
	public:
		void OnStart() override
		{
			CommandBuffer_ = AllocEmptyCString(COMMAND_BUFFER_LENGTH);
		}

		void OnEnd() override
		{
			delete[] CommandBuffer_;
		}

		void OnRender(dcore::graphics::RendererInterface *renderer) override
		{
			(void)renderer;

			ImGui::Begin("[DEBUG] Frame Log");
			const auto &queue = dcore::FrameLog::Instance()->GetQueue();
			for(const auto &s : queue) ImGui::Text("%s", s.c_str());
			ImGui::End();

			float spacing = ImGui::GetTextLineHeightWithSpacing()
				+ ImGui::GetStyle().FramePadding.y
				+ ImGui::GetStyle().ItemSpacing.y;

			ImGui::Begin("[DEBUG] Console");
			ImGui::BeginChild("##Console_ScrollRegion", ImVec2(0, -spacing), false, ImGuiWindowFlags_AlwaysHorizontalScrollbar);
			const auto &messages = dstd::ILog()->GetMessages();
			for(dstd::USize index = 0; index < messages.GetSize(); ++index)
				ImGui::Text("%s", messages[index].c_str());
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			ImGui::EndChild();
			ImGui::InputText("##Console_Command", CommandBuffer_, COMMAND_BUFFER_LENGTH - 1);
			ImGui::SameLine();
			if(ImGui::Button("Enter"))
			{
				bool success = true;
				std::string_view cmd { CommandBuffer_ };

				if(cmd.find("help") == 0)
				{
					LOG_F(INFO, "Help");
				}
				else
				{
					LOG_F(ERROR, "Unknown Command: '%s'", CommandBuffer_);
					success = false;
				}

				if(success) std::fill(CommandBuffer_, CommandBuffer_ + COMMAND_BUFFER_LENGTH, 0);
			}

			ImGui::End();
		}
	};
}

namespace dg::loaders
{
	MainWorldLoader::MainWorldLoader(std::string name) : Name_(std::move(name)) {}

	void MainWorldLoader::Load(dcore::world::World *world)
	{
		// Read the world data from a json file.
		dcore::data::FileInput input("data", "dcore/main/worlds/" + Name_ + ".json");
		dcore::data::adapters::JsonInputAdapter jsonAdapter;
		input.SetAdapter(&jsonAdapter);
		input.Read();

		// Add all of the necessary entities to the world
		PopulateWorld_(input, world);
	}

	void MainWorldLoader::UnLoad(dcore::world::World *world) {}

	void MainWorldLoader::PopulateWorld_(const dcore::data::FileInput &input, dcore::world::World *world)
	{
		world->CreateTerrain(dcore::resource::GetResource<dcore::terrain::Heightmap>("DCore.Heightmap." + Name_));

		// Load the rest of the entities (saved)
		LoadStaticEntities_({input.Get(), world});

		auto player = world->CreateEntity();
		auto *renderer = dcore::platform::Context::Instance()->GetRendererInterface();
		player.AddComponent(dcore::world::TransformComponent());
		player.GetComponent<dcore::world::TransformComponent>()->SetPosition(glm::vec3(0, 0, 0));
		player.GetComponent<dcore::world::TransformComponent>()->SetRotation(glm::identity<glm::quat>());
		player.GetComponent<dcore::world::TransformComponent>()->SetScale(glm::vec3(1, 1, 1));
		player.AddComponent(dcore::world::StaticMeshComponent(dcore::graphics::StaticMesh(
			    dcore::resource::GetResource<dcore::graphics::RStaticMesh>("DCore.Mesh.Cube"),
			    dcore::resource::GetResource<dcore::graphics::RTexture>("DCore.Texture.Main.Stone"))));
		player.AddComponent(entity::CharacterControllerComponent());
		player.AddComponent(entity::CameraFollowComponent(renderer->GetCamera()));

		world->AddDebugLayer(new BaseDebugLayer());
		world->AddDebugLayer(new ConsoleDebugLayer());

#if 0 // Future API?
		auto server = dg::net::ServerInterface::Instance();

		dstd::UInt32 entityCount = 0;
		server->GetAllEntities(&entityCount, nullptr, Name_);
		std::vector<dg::net::EntityCreateData> entityData(entityCount);
		server->GetAllEntities(nullptr, &entityData[0], Name_);

		this->ConstructEntities_(entityData);
#endif
	}

	void MainWorldLoader::LoadStaticEntities_(LoadInfo info)
	{
		(void)Name_;

		const auto &objsJson = info.Json["statics"];
		for(const auto &obj : objsJson)
		{
			LOG_F(INFO, "Creating entity");
			auto entity = info.World->CreateEntity();

			dstd::UUID uuid;
			dstd::UUID::Parse(uuid, obj["uuid"]);
			entity.AddComponent(dcore::world::UUIDComponent(uuid));

			if(obj["type"] == "Model") LoadStaticEntity_Model_(entity, {obj, info.World});
			if(obj["type"] == "NPC") LoadStaticEntity_NPC_(entity, {obj, info.World});
			if(obj["type"] == "Interactable") LoadStaticEntity_Interactable_(entity, {obj, info.World});

			LOG_F(INFO, "Created entity! %u", entity.GetId());
		}
	}

	void MainWorldLoader::LoadStaticEntity_Transform_(dcore::world::Entity &entity, LoadInfo info)
	{
		(void)Name_;

		glm::vec3 position = {
		    info.Json["position"][0].get<float>(),
		    info.Json["position"][1].get<float>(),
		    info.Json["position"][2].get<float>(),
		};

		glm::quat rotation = {
		    info.Json["rotation"][3].get<float>(),
		    info.Json["rotation"][0].get<float>(),
		    info.Json["rotation"][1].get<float>(),
		    info.Json["rotation"][2].get<float>(),
		};

		glm::vec3 scale = {
		    info.Json["scale"][0].get<float>(),
		    info.Json["scale"][1].get<float>(),
		    info.Json["scale"][2].get<float>(),
		};

		dcore::world::TransformComponent transform;
		transform.SetPosition(position);
		transform.SetRotation(rotation);
		transform.SetScale(scale);
		transform.ReCalculateMatrix(); // faster

		entity.AddComponent(transform);
	}

	void MainWorldLoader::LoadStaticEntity_Model_(dcore::world::Entity &entity, LoadInfo info)
	{
		(void)Name_;
		LoadStaticEntity_Transform_(entity, info);

		LOG_F(INFO, "Creating model entity");

		auto modelId = info.Json["model"].get<const std::string>();

		auto model = dcore::resource::ResourceManager::Instance()->Get<dcore::graphics::Model>(modelId);
		entity.AddComponent(dcore::world::ModelComponent(model));
	}

	void MainWorldLoader::LoadStaticEntity_NPC_(dcore::world::Entity &entity, LoadInfo info) { (void)Name_; }

	void MainWorldLoader::LoadStaticEntity_Interactable_(dcore::world::Entity &entity, LoadInfo info) { (void)Name_; }
} // namespace dg::loaders
