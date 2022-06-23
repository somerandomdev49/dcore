#include <dg/Editor.hpp>

#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/World/World.hpp>

#include <dg/Loaders/MainWorldLoader.hpp>
#include <dg/Editor/Layers.hpp>

namespace dg
{
	void Editor::Initialize()
	{
		auto *mainWorldLoader = new dg::loaders::MainWorldLoader("World1");
		Registry_.AddWorldLoader("Main1", mainWorldLoader);
		Registry_.LoadWorld("Main1");
		
		auto *rend = dcore::platform::Context::Instance()->GetRendererInterface();
		auto *world = dcore::platform::Context::Instance()->GetWorld();
		world->AddDebugLayer(new editor::EditorLayer(mainWorldLoader->GetPlayer()));
		world->AddDebugLayer(new editor::FramebufferLayer());
		world->AddDebugLayer(new editor::ConsoleLayer());
		world->AddDebugLayer(new editor::ResourceBrowserLayer());
		
#define FOG_COLOR 0.23f, 0.48f, 0.74f, 1.0f
		rend->GetRenderer()->RenderToFramebuffer(true);
		rend->GetRenderer()->SetClearColor({ FOG_COLOR });
	}

	void Editor::DeInitialize()
	{
		Registry_.UnLoadAllWorlds();
		for(auto &[name, loader] : Registry_.GetLoaders())
			delete loader;
	}
}
