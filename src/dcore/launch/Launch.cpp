#include <dcore/Launch.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Log.hpp>

#define LOG_STMT(STMT)                            \
	DCORE_LOG_INFO << __LINE__ << " | " << #STMT; \
	STMT;

namespace dcore {
	void launch::Launch::Run(int argc, char *argv[]) {
		// (void)argc; (void)argv;
		loguru::g_stderr_verbosity = 1;
		loguru::g_preamble_date    = false;
		loguru::g_preamble_time    = false;
		loguru::g_preamble_thread  = false;
		loguru::init(argc, argv);
		platform::PlatformSpecific ps;
		(void)ps;
		LOG_STMT(platform::Context ctx);

		LOG_STMT(world::World world;)
		LOG_STMT(graphics::Renderer rend;)

		LOG_STMT(ctx.Rend_ = &rend;)
		LOG_STMT(ctx.World_ = &world;)

		LOG_STMT(ctx.Initialize();)

		LOG_STMT(event::InputManager im;)
		LOG_STMT(im.Initialize();)
		LOG_STMT(event::InputManager::SetInstance(&im);)

		LOG_STMT(world.Initialize();)

		// Create the default config reader.
		LOG_STMT(resource::ConfigReader cfg("data");)
		LOG_STMT(resource::ConfigReader::SetDefaultReader(&cfg);)
		LOG_STMT(resource::ResourceLoader rl("data");)
		LOG_STMT(resource::ResourceManager rm("data");)
		LOG_STMT(rm.Initialize();)
		LOG_STMT(rl.LoadMappings("ResourceMap.ini");)
		LOG_STMT(rl.LoadFromManifest("Manifest.cfg");)

		LOG_STMT(world::Entity e = world.CreateEntity();)
		LOG_STMT(e.AddComponent(world::TransformComponent());)
		LOG_STMT(e.AddComponent(world::StaticMeshComponent {
		    graphics::StaticMesh(rm.Get<graphics::RStaticMesh>("DCore.Mesh.Cube"),
		                         rm.Get<graphics::RTexture>("DCore.Texture.Main.Grass"))});)

		// e.AddComponent(MyComponent());
		// world.RegisterUpdate([](world::World *c)
		// {
		//     c->Each<world::MyComponent>([](world::Entity *e, world::MyComponent *comp)
		//     {
		//         /**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_A))
		//         {
		//             auto cam =
		//             platform::Context::Instance()->GetRendererInterface()->GetCamera(); auto pos
		//             = cam->GetPosition(); pos.x -= 0.1f; cam->SetPosition(pos);
		//         }
		//         else if(event::InputManager::Instance()->IsKeyPressed(event::K_D))
		//         {
		//             auto cam =
		//             platform::Context::Instance()->GetRendererInterface()->GetCamera(); auto pos
		//             = cam->GetPosition(); pos.x += 0.1f; cam->SetPosition(pos);
		//         }

		//         /**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_W))
		//         {
		//             auto cam =
		//             platform::Context::Instance()->GetRendererInterface()->GetCamera(); auto pos
		//             = cam->GetPosition(); pos.z -= 0.1f; cam->SetPosition(pos);
		//         }
		//         if(event::InputManager::Instance()->IsKeyPressed(event::K_S))
		//         {
		//             auto cam =
		//             platform::Context::Instance()->GetRendererInterface()->GetCamera(); auto pos
		//             = cam->GetPosition(); pos.z += 0.1f; cam->SetPosition(pos);
		//         }
		//     });
		// });

		// e.GetComponent<world::ModelRenderableComponent>().Mesh.SetTransform(e.GetComponent<world::TransformComponent>().CalculateMatrix());
		LOG_STMT(e.GetComponent<world::TransformComponent>().Position = glm::vec3(0, -1.0f, -6.0f);)
		LOG_STMT(e.GetComponent<world::TransformComponent>().Rotation = glm::identity<glm::quat>();)
		LOG_STMT(e.GetComponent<world::TransformComponent>().Scale = glm::vec3(1.0f, 1.0f, 1.0f);)

		LOG_STMT(ctx.DefaultResourceInit(&rm);)
		LOG_STMT(ctx.Start();)
		LOG_STMT(ctx.CloseWindow();)

		DCORE_LOG_INFO << "----------------------------------------";

		rm.DeInitialize();
		im.DeInitialize();
		world.DeInitialize();
		ctx.DeInitialize();
	}
} // namespace dcore
