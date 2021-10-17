#include <dcore/Launch.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Log.hpp>

#define STMT                                      \
	DCORE_LOG_INFO << __LINE__ << " | " << #STMT; \
	STMT;

namespace dcore
{
	void launch::Launch::Run(int argc, char *argv[])
	{
		// (void)argc; (void)argv;
		loguru::g_stderr_verbosity = 1;
		loguru::g_preamble_date    = false;
		loguru::g_preamble_time    = false;
		loguru::g_preamble_thread  = false;
		loguru::init(argc, argv);
		platform::PlatformSpecific ps;
		(void)ps;
		platform::Context ctx;

		world::World world;
		graphics::Renderer rend;

		ctx.Rend_  = &rend;
		ctx.World_ = &world;

		ctx.Initialize();
		platform::Context::SetInstance(&ctx);

		event::InputManager im;
		im.Initialize();
		event::InputManager::SetInstance(&im);

		event::TimeManager tm;
		tm.Initialize();
		event::TimeManager::SetInstance(&tm);

		// Create the default config reader.
		resource::ConfigReader cfg("data");
		resource::ConfigReader::SetDefaultReader(&cfg);
		resource::ResourceLoader rl("data");
		resource::ResourceManager rm("data");
		resource::ResourceManager::SetInstance(&rm);
		rm.Initialize();
		graphics::RenderResourceManager::Register(&rl);
		terrain::TerrainResourceManager::Register(&rl);
		rl.LoadMappings("ResourceMap.ini");
		rl.LoadFromManifest("Manifest.cfg");

		world.Initialize();
		world::Entity e = world.CreateEntity();
		e.AddComponent(world::TransformComponent());
		// e.AddComponent(world::StaticMeshComponent {
		//     graphics::StaticMesh(rm.Get<graphics::RStaticMesh>("DCore.Mesh.Cube"), rm.Get<graphics::RTexture>("DCore.Texture.Main.Grass"))});

		struct MyComponent
		{
			int nonEmpty;
		};

		e.AddComponent(MyComponent());
		world.RegisterUpdate([](world::World *c) {
			c->Each<MyComponent>([](world::Entity *e, MyComponent *comp) {
				float speed = 10.f * event::TimeManager::Instance()->GetDeltaTime();
				float rotSpeed = 1.f * event::TimeManager::Instance()->GetDeltaTime();
				auto cam = platform::Context::Instance()->GetRendererInterface()->GetCamera();
				// printf("Delta Time: %f\n", event::TimeManager::Instance()->GetDeltaTime());

				/**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_A))
				{
					auto pos = cam->GetPosition();
					pos += cam->GetRotation() * glm::vec3(-speed, 0, 0);
					cam->SetPosition(pos);
				}
				else if(event::InputManager::Instance()->IsKeyPressed(event::K_D))
				{
					auto pos = cam->GetPosition();
					pos += cam->GetRotation() * glm::vec3(speed, 0, 0);
					cam->SetPosition(pos);
				}

				/**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_W))
				{
					auto pos = cam->GetPosition();
					pos += cam->GetRotation() * glm::vec3(0, 0, -speed);
					cam->SetPosition(pos);
				}
				else if(event::InputManager::Instance()->IsKeyPressed(event::K_S))
				{
					auto pos = cam->GetPosition();
					pos += cam->GetRotation() * glm::vec3(0, 0, speed);
					cam->SetPosition(pos);
				}

				/**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_LeftShift))
				{
					auto pos = cam->GetPosition();
					pos += cam->GetRotation() * glm::vec3(0, -speed, 0);
					cam->SetPosition(pos);
				}
				else if(event::InputManager::Instance()->IsKeyPressed(event::K_Space))
				{
					auto pos = cam->GetPosition();
					pos += cam->GetRotation() * glm::vec3(0, speed, 0);
					cam->SetPosition(pos);
				}

				/**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_Q))
				{
					auto rot = cam->GetRotation();
					rot = glm::angleAxis(rotSpeed, glm::vec3(0, 1, 0)) * rot;
					cam->SetRotation(rot);
				}
				else if(event::InputManager::Instance()->IsKeyPressed(event::K_E))
				{
					auto rot = cam->GetRotation();
					rot = glm::angleAxis(-rotSpeed, glm::vec3(0, 1, 0)) * rot;
					cam->SetRotation(rot);
				}
			});
		});

		// e.GetComponent<world::ModelRenderableComponent>().Mesh.SetTransform(e.GetComponent<world::TransformComponent>().CalculateMatrix());
		e.GetComponent<world::TransformComponent>().Position = glm::vec3(0, -1.0f, -6.0f);
		e.GetComponent<world::TransformComponent>().Rotation = glm::identity<glm::quat>();
		e.GetComponent<world::TransformComponent>().Scale    = glm::vec3(1.0f, 1.0f, 1.0f);

		DCORE_LOG_WARNING << "Starting...";
		ctx.DefaultResourceInit(&rm);

		// ctx.GetRendererInterface()->GetCamera()->SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		// ctx.GetRendererInterface()->GetRenderer()->SetWireframeMode(true);

		ctx.Start();
		ctx.CloseWindow();

		DCORE_LOG_INFO << "----------------------------------------";

		rm.DeInitialize();
		im.DeInitialize();
		tm.DeInitialize();
		world.DeInitialize();
		ctx.DeInitialize();
	}
} // namespace dcore
