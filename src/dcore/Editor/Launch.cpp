#include <dcore/Launch.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Log.hpp>

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

		ctx.Rend_     = &rend;
		ctx.World_    = &world;

		ctx.Initialize();
		platform::Context::SetInstance(&ctx);

		event::InputManager im;
		im.Initialize();
		event::InputManager::SetInstance(&im);

		event::TimeManager tm;
		tm.Initialize();
		event::TimeManager::SetInstance(&tm);

		graphics::gui::Font::FontLibInitialize();

		// Create the default config reader.
		resource::ConfigReader cfg("data");
		resource::ConfigReader::SetDefaultReader(&cfg);
		resource::ResourceLoader rl("data");
		resource::ResourceManager rm("data");
		resource::ResourceManager::SetInstance(&rm);
		rm.Initialize();
		graphics::RenderResourceManager::Register(&rl);
		terrain::TerrainResourceManager::Register(&rl);
		graphics::gui::FontResourceManager::Register(&rl);
		rl.LoadMappings("ResourceMap.ini");
		rl.LoadFromManifest("Manifest.cfg");

		world.Initialize();
		world::Entity e = world.CreateEntity();
		e.AddComponent(world::TransformComponent());
		// e.AddComponent(world::StaticMeshComponent {
		//     graphics::StaticMesh(rm.Get<graphics::RStaticMesh>("DCore.Mesh.Cube"),
		//     rm.Get<graphics::RTexture>("DCore.Texture.Main.Grass"))});

		struct MyComponent
		{
			int nonEmpty;
		};

		e.AddComponent(MyComponent());
		world.RegisterUpdate(
		    [](world::World *c)
		    {
			    c->Each<MyComponent>(
			        [](world::Entity *e, MyComponent *comp)
			        {
                        auto worldInstance = platform::Context::Instance()->GetWorld();

				        (void)e;
				        (void)comp;
				        float speed    = 10.f * event::TimeManager::Instance()->GetDeltaTime();
				        float rotSpeed = 1.f * event::TimeManager::Instance()->GetDeltaTime();
				        auto cam       = platform::Context::Instance()->GetRendererInterface()->GetCamera();
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
					        rot      = glm::angleAxis(rotSpeed, glm::vec3(0, 1, 0)) * rot;
					        cam->SetRotation(rot);
				        }
				        else if(event::InputManager::Instance()->IsKeyPressed(event::K_E))
				        {
					        auto rot = cam->GetRotation();
					        rot      = glm::angleAxis(-rotSpeed, glm::vec3(0, 1, 0)) * rot;
					        cam->SetRotation(rot);
				        }

				        float rdOffset = 30.f;
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_0))
					        worldInstance->SetRenderDistance(rdOffset + 100.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_1))
					        worldInstance->SetRenderDistance(rdOffset + 10.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_2))
					        worldInstance->SetRenderDistance(rdOffset + 20.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_3))
					        worldInstance->SetRenderDistance(rdOffset + 30.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_4))
					        worldInstance->SetRenderDistance(rdOffset + 40.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_5))
					        worldInstance->SetRenderDistance(rdOffset + 50.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_6))
					        worldInstance->SetRenderDistance(rdOffset + 60.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_7))
					        worldInstance->SetRenderDistance(rdOffset + 70.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_8))
					        worldInstance->SetRenderDistance(rdOffset + 80.f);
				        if(event::InputManager::Instance()->IsKeyPressed(event::K_9))
					        worldInstance->SetRenderDistance(rdOffset + 90.f);
			        });
		    });

		// e.GetComponent<world::ModelRenderableComponent>().Mesh.SetTransform(e.GetComponent<world::TransformComponent>().CalculateMatrix());
		e.GetComponent<world::TransformComponent>().Position = glm::vec3(0, -1.0f, -6.0f);
		e.GetComponent<world::TransformComponent>().Rotation = glm::identity<glm::quat>();
		e.GetComponent<world::TransformComponent>().Scale    = glm::vec3(1.0f, 1.0f, 1.0f);

		DCORE_LOG_WARNING << "Starting...";
		ctx.DefaultResourceInit(&rm);

		graphics::gui::GuiGraphics guig;
		guig.Initialize(resource::ResourceManager::Instance());
		graphics::gui::GuiGraphics::SetInstance(&guig);

		// ctx.GetRendererInterface()->GetCamera()->SetRotation(glm::quat(glm::vec3(0, 0, 0)));
		// ctx.GetRendererInterface()->GetRenderer()->SetWireframeMode(true);

		ctx.Start();
		ctx.CloseWindow();

		DCORE_LOG_INFO << "----------------------------------------";

		rm.DeInitialize();
		im.DeInitialize();
		tm.DeInitialize();
		graphics::gui::Font::FontLibDeInitialize();
		world.DeInitialize();
		guig.DeInitialize();
		ctx.DeInitialize();
	}
}
