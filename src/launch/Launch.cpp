#include <dcore/Launch.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Log.hpp>

namespace dcore
{
    void launch::Launch::Run(int argc, char *argv[])
    {
        // (void)argc; (void)argv;
        loguru::g_stderr_verbosity = 1;
        loguru::g_preamble_date = false;
        loguru::g_preamble_time = false;
        loguru::g_preamble_thread = false;
        loguru::init(argc, argv);
        platform::PlatformSpecific ps; (void)ps;
        platform::Context ctx;

        world::World world;
        graphics::Renderer rend;

        ctx.Rend_ = &rend;
        ctx.World_ = &world;

        ctx.Initialize();

        event::InputManager im;
        im.Initialize();
        event::InputManager::SetInstance(&im);

        world.Initialize();

        // Create the default config reader.
        resource::ConfigReader cfg("data");
        resource::ConfigReader::SetDefaultReader(&cfg);
        resource::ResourceLoader rl("data");
        resource::ResourceManager rm("data");
        rm.Initialize();
        rl.LoadMappings("ResourceMap.ini");
        rl.LoadFromManifest("Manifest.cfg");

        world::Entity e = world.CreateEntity();
        e.AddComponent(world::TransformComponent());
        e.AddComponent(world::StaticMeshComponent{
            graphics::StaticMesh(
                rm.Get<graphics::RStaticMesh>("DCore.Mesh.Cube"),
                rm.Get<graphics::RTexture>("DCore.Texture.Main.Grass")
            )
        });

        // e.AddComponent(MyComponent());
        // world.RegisterUpdate([](world::World *c)
        // {
        //     c->Each<world::MyComponent>([](world::Entity *e, world::MyComponent *comp)
        //     {
        //         /**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_A))
        //         {
        //             auto cam = platform::Context::Instance()->GetRendererInterface()->GetCamera();
        //             auto pos = cam->GetPosition();
        //             pos.x -= 0.1f;
        //             cam->SetPosition(pos);
        //         }
        //         else if(event::InputManager::Instance()->IsKeyPressed(event::K_D))
        //         {
        //             auto cam = platform::Context::Instance()->GetRendererInterface()->GetCamera();
        //             auto pos = cam->GetPosition();
        //             pos.x += 0.1f;
        //             cam->SetPosition(pos);
        //         }
                
        //         /**/ if(event::InputManager::Instance()->IsKeyPressed(event::K_W))
        //         {
        //             auto cam = platform::Context::Instance()->GetRendererInterface()->GetCamera();
        //             auto pos = cam->GetPosition();
        //             pos.z -= 0.1f;
        //             cam->SetPosition(pos);
        //         }
        //         if(event::InputManager::Instance()->IsKeyPressed(event::K_S))
        //         {
        //             auto cam = platform::Context::Instance()->GetRendererInterface()->GetCamera();
        //             auto pos = cam->GetPosition();
        //             pos.z += 0.1f;
        //             cam->SetPosition(pos);
        //         }
        //     });
        // });

        // e.GetComponent<world::ModelRenderableComponent>().Mesh.SetTransform(e.GetComponent<world::TransformComponent>().CalculateMatrix());
        e.GetComponent<world::TransformComponent>().Position = glm::vec3(0, -1.0f, -6.0f);
        e.GetComponent<world::TransformComponent>().Rotation = glm::identity<glm::quat>();//glm::angleAxis(glm::degrees(15.0f), glm::vec3(0, 1, 0));
        e.GetComponent<world::TransformComponent>().Scale = glm::vec3(1.0f, 1.0f, 1.0f);

        ctx.DefaultResourceInit(&rm);
        ctx.Start();
        ctx.CloseWindow();

        DCORE_LOG_INFO << "----------------------------------------";

        rm.DeInitialize();
        im.DeInitialize();
        world.DeInitialize();
        ctx.DeInitialize();
    }
}
