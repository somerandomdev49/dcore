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

        // Create the default config reader.
        resource::ConfigReader cfg("data");
        resource::ConfigReader::SetDefaultReader(&cfg);
        resource::ResourceLoader rl("data");
        resource::ResourceManager rm;
        rm.Initialize();
        rl.LoadMappings("ResourceMap.ini");
        rl.LoadManifest("Manifest.cfg", &rm);

        world::Entity e = world.CreateEntity();
        e.AddComponent(world::TransformComponent());
        e.AddComponent(world::ModelRenderableComponent{
            graphics::StaticMesh(
                rm.Get<fwdraw::Mesh>("DCore.Mesh.Cube"),
                rm.Get<fwdraw::Texture>("DCore.Texture.Main.Grass")
            )
        });

        // e.GetComponent<world::ModelRenderableComponent>().Mesh.SetTransform(e.GetComponent<world::TransformComponent>().CalculateMatrix());
        e.GetComponent<world::TransformComponent>().Position = glm::vec3(0, -1.0f, -6.0f);
        e.GetComponent<world::TransformComponent>().Rotation = glm::identity<glm::quat>();//glm::angleAxis(glm::degrees(15.0f), glm::vec3(0, 1, 0));
        e.GetComponent<world::TransformComponent>().Scale = glm::vec3(1.0f, 1.0f, 1.0f);

        ctx.DefaultResourceInit(&rm);
        ctx.Start();
        ctx.CloseWindow();

        DCORE_LOG_INFO << "----------------------------------------";

        rm.DeInitialize();
        ctx.DeInitialize();
    }
}
