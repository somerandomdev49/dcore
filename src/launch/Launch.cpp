#include <dcore/Launch.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>

namespace dcore
{
    void launch::Launch::Run(int argc, char *argv[])
    {
        (void)argc; (void)argv;

        // Create the default config reader.
        resource::ConfigReader cfg("data");
        resource::ConfigReader::SetDefaultReader(&cfg);
        resource::ResourceLoader rl("data");
        resource::ResourceManager rm;
        rm.Initialize();
        rl.LoadManifest("Manifest.cfg", &rm);
        rm.DeInitialize();
    }
}
