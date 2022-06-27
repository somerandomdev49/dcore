#include <dg/Editor/Layers.hpp>

#include <dcore/Graphics/Graphics.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/Type.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/FrameLog.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Resource/ResourceManager.hpp>

#include <dg/Loaders/MainWorldLoader.hpp>
#include <imgui.h>

namespace dg::editor
{
	void ResourceBrowserLayer::OnStart() {}

	void ResourceBrowserLayer::OnEnd() {}

	void ResourceBrowserLayer::OnRender(dcore::graphics::RendererInterface *renderer)
	{
		ImGui::Begin("Resource Browser");
		const auto &resources = dcore::resource::ResourceManager::Instance()->GetResources();
		for(const auto &[type, map] : resources)
		{
			for(const auto &[name, resource] : map)
			{
				ImGui::Text("[%s] %s", dcore::util::Debug::Demangle(type.name()).c_str(), name.c_str());
			}
		}
		ImGui::End();
	}

} // namespace dg::editor
