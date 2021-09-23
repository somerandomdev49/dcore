#include <dcore/Launch.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Core/Log.hpp>

namespace dcore
{
    void launch::Launch::Run(int argc, char *argv[])
    {
        // (void)argc; (void)argv;
        loguru::init(argc, argv);
        loguru::g_stderr_verbosity = 1;

        // Create the default config reader.
        resource::ConfigReader cfg("data");
        resource::ConfigReader::SetDefaultReader(&cfg);
        resource::ResourceLoader rl("data");
        resource::ResourceManager rm;
        rm.Initialize();
        rl.LoadMappings("ResourceMap.ini");
        rl.LoadManifest("Manifest.cfg", &rm);
        rm.DeInitialize();
    }
}
