#include <dcore/Launch.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Core/Preferences.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Log.hpp>
#include <dg/Game.hpp>
#include <dg/Editor.hpp>
#include <iostream>

#define LOGGER_MESSAGE 512

namespace dcore::launch
{
	void Launch::Run(int argc, char **argv)
	{
		dstd::Log logger(LOGGER_MESSAGE);
		dstd::ILog(&logger);
		loguru::g_stderr_verbosity = 1;
		loguru::g_preamble_date    = false;
		loguru::g_preamble_time    = false;
		loguru::g_preamble_thread  = false;
		logger.Initialize();
		loguru::init(argc, argv);

		printf("huh?\n");

		static const char *prefPath = "preferences.json";

		static const auto LoadPrefs = [](Preferences *pref)
		{
			Preferences::SetInstance(pref);
			data::FileInput fileInput("saves", prefPath);
			data::adapters::JsonInputAdapter jsonAdapter;
			fileInput.SetAdapter(&jsonAdapter);
			fileInput.Read();
			pref->Read(fileInput);
		};

		static const auto InitContext = [](platform::Context *ctx, graphics::Renderer *rend)
		{
			platform::PlatformSpecific specific;
			(void)specific;
			ctx->Rend_  = rend;
			ctx->World_ = nullptr;
			ctx->Initialize();
			platform::Context::SetInstance(ctx);
		};

		static const auto InitEventManagers = [](event::InputManager *imngr, event::TimeManager *tmngr)
		{
			event::InputManager::SetInstance(imngr);
			event::TimeManager::SetInstance(tmngr);
			imngr->Initialize();
			tmngr->Initialize();
		};

		static const auto InitResources = [](resource::ResourceManager *rmngr, resource::ResourceLoader *rloader)
		{
			// Create the default config reader.
			resource::ConfigReader cfg("data");
			resource::ConfigReader::SetDefaultReader(&cfg);
			resource::ResourceManager::SetInstance(rmngr);
			rmngr->Initialize();
			graphics::RenderResourceManager::Register(rloader);
			terrain::TerrainResourceManager::Register(rloader);
			graphics::gui::FontResourceManager::Register(rloader);
			platform::Frame::RegisterResourceManager(rloader);
			rloader->LoadMappings("ResourceMap.ini");
			rloader->LoadFromManifest("Manifest.cfg");
		};

		static const auto InitGUI = [](graphics::gui::GuiGraphics *guig, graphics::gui::GuiManager *guimngr)
		{
			graphics::gui::GuiGraphics::SetInstance(guig);
			guig->Initialize(resource::ResourceManager::Instance());
			graphics::gui::GuiManager::SetInstance(guimngr);
			guimngr->Initialize();
		};

		Preferences prefs;
		platform::Context context;
		// world::World world;
		graphics::Renderer renderer;
		resource::ResourceManager resourceManager("data");
		resource::ResourceLoader resourceLoader("data");
		event::InputManager inputManager;
		event::TimeManager timeManager;
		graphics::gui::GuiManager guiManager;
		graphics::gui::GuiGraphics guiGraphics;
		FrameLog frameLog;
		FrameLog::SetInstance(&frameLog);

		graphics::gui::Font::FontLibInitialize();

		LoadPrefs(&prefs);
		InitContext(&context, &renderer);
		InitEventManagers(&inputManager, &timeManager);
		InitResources(&resourceManager, &resourceLoader);
		InitGUI(&guiGraphics, &guiManager);

		// platform::Context::Instance()->GetWorld()->Initialize();
		platform::Context::Instance()->DefaultResourceInit();

// #define DCORE_USE_EDITOR
#ifdef DCORE_USE_EDITOR
		dg::Editor
#else
		dg::Game
#endif
		game;
		
		// dg::Game::SetInstance(&game);
		game.Initialize();

		DCORE_LOG_WARNING << "======================== Starting ========================";
		platform::Context::Instance()->Start();

		DCORE_LOG_INFO << "======================== Exiting ========================";

		game.DeInitialize();

		{
			// data::FileOutput fileOutput("saves", "save1.json");
			// data::adapters::JsonOutputAdapter jsonAdapter;
			// fileOutput.SetAdapter(&jsonAdapter);
			// platform::Context::Instance()->GetWorld()->Save(fileOutput);
			// fileOutput.Write();
		}

		inputManager.DeInitialize();
		DCORE_LOG_INFO << "input manager deinitialized!";
		timeManager.DeInitialize();
		DCORE_LOG_INFO << "time manager deinitialized!";
		guiManager.DeInitialize();
		guiGraphics.DeInitialize();
		resourceManager.DeInitialize();
		graphics::gui::Font::FontLibDeInitialize();
		DCORE_LOG_INFO << "Resource Manager deinitialized!";
		context.DeInitialize();

		logger.DeInitialize();
		loguru::remove_all_callbacks();
	}
} // namespace dcore::launch
