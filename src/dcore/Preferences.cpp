#include <dcore/Core/Preferences.hpp>

namespace dcore
{
	void Preferences::Read(const dcore::data::FileInput &input)
	{
		Version_ = input.Get()["version"].get<int>();
		switch(Version_)
		{
		case 1:
		{
			const auto &display              = input.Get()["display"];
			DisplaySettings_.Resolution.x    = display["resolution"][0].get<int>();
			DisplaySettings_.Resolution.y    = display["resolution"][1].get<int>();
			DisplaySettings_.Fullscreen      = display["fullscreen"].get<bool>();
			const auto &graphics             = input.Get()["graphics"];
			GraphicsSettings_.RenderDistance = graphics["render-distance"].get<int>();
		}
		}
	}
} // namespace dcore
