#include <dcore/Launch.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Platform.hpp>
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
        platform::PlatformSpecific ps;
        platform::Context ctx;
        ctx.Initialize();

        // Create the default config reader.
        resource::ConfigReader cfg("data");
        resource::ConfigReader::SetDefaultReader(&cfg);
        resource::ResourceLoader rl("data");
        resource::ResourceManager rm;
        rm.Initialize();
        rl.LoadMappings("ResourceMap.ini");
        rl.LoadManifest("Manifest.cfg", &rm);

        ctx.OpenWindow();
        ctx.CloseWindow();

        rm.DeInitialize();
        ctx.DeInitialize();
    }
}
