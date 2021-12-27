#include <dcore/Launch.hpp>
#include <dcore/Data/Adapters/JsonAdapter.hpp>
#include <dcore/Resource/ConfigReader.hpp>
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Resource/ResourceLoader.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Graphics/GUI/GuiGraphics.hpp>
#include <dcore/Graphics/GUI/GuiManager.hpp>
#include <dcore/Graphics/GUI/Font.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Core/Preferences.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Log.hpp>
#include <dg/Game.hpp>
#include <iostream>

namespace dcore::launch
{
	void Launch::Run(int argc, char *argv[])
	{
		loguru::g_stderr_verbosity = 1;
		loguru::g_preamble_date    = false;
		loguru::g_preamble_time    = false;
		loguru::g_preamble_thread  = false;
		loguru::init(argc, argv);

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

		static const auto InitContext =
		    [](platform::Context *ctx, world::World *world, graphics::Renderer *rend)
		{
			platform::PlatformSpecific ps;
			(void)ps;
			ctx->Rend_  = rend;
			ctx->World_ = world;
			ctx->Initialize();
			platform::Context::SetInstance(ctx);
		};

		static const auto InitEventManagers = [](event::InputManager *im, event::TimeManager *tm)
		{
			event::InputManager::SetInstance(im);
			event::TimeManager::SetInstance(tm);
			im->Initialize();
			tm->Initialize();
		};

		static const auto InitResources =
		    [](resource::ResourceManager *rm, resource::ResourceLoader *rl)
		{
			// Create the default config reader.
			resource::ConfigReader cfg("data");
			resource::ConfigReader::SetDefaultReader(&cfg);
			resource::ResourceManager::SetInstance(rm);
			rm->Initialize();
			graphics::RenderResourceManager::Register(rl);
			terrain::TerrainResourceManager::Register(rl);
			graphics::gui::FontResourceManager::Register(rl);
			rl->LoadMappings("ResourceMap.ini");
			rl->LoadFromManifest("Manifest.cfg");
		};

		static const auto InitGUI =
		    [](graphics::gui::GuiGraphics *guig, graphics::gui::GuiManager *guimngr)
		{
			graphics::gui::GuiGraphics::SetInstance(guig);
			guig->Initialize(resource::ResourceManager::Instance());
			graphics::gui::GuiManager::SetInstance(guimngr);
			guimngr->Initialize();
		};

		Preferences prefs;
		platform::Context context;
		world::World world;
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
		InitContext(&context, &world, &renderer);
		InitEventManagers(&inputManager, &timeManager);
		InitResources(&resourceManager, &resourceLoader);
		InitGUI(&guiGraphics, &guiManager);

		platform::Context::Instance()->GetWorld()->Initialize();
		platform::Context::Instance()->DefaultResourceInit();

		dg::Game game;
		// dg::Game::SetInstance(&game);
		game.Initialize();

		DCORE_LOG_WARNING << "======================== Starting ========================";
		platform::Context::Instance()->Start();

		DCORE_LOG_INFO << "======================== Exiting ========================";

		game.DeInitialize();

		{
			data::FileOutput fileOutput("saves", "save1.json");
			data::adapters::JsonOutputAdapter jsonAdapter;
			fileOutput.SetAdapter(&jsonAdapter);
			platform::Context::Instance()->GetWorld()->Save(fileOutput);
			fileOutput.Write();
		}

		resourceManager.DeInitialize();
		inputManager.DeInitialize();
		timeManager.DeInitialize();
		graphics::gui::Font::FontLibDeInitialize();
		guiManager.DeInitialize();
		guiGraphics.DeInitialize();
		world.DeInitialize();
		context.DeInitialize();
	}
} // namespace dcore::launch
