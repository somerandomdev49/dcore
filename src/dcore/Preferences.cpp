#include <dcore/Core/Preferences.hpp>
#include <dcore/Core/Log.hpp>

namespace dcore
{
	void Preferences::Read(const dcore::data::FileInput &input)
	{
		Version_      = input.Get()["version"].get<int>();
		auto settings = input.Get()["settings"];
		switch(Version_)
		{
		case 1:
		{
			const auto &display              = settings["display"];
			DisplaySettings_.Resolution.x    = display["resolution"][0].get<int>();
			DisplaySettings_.Resolution.y    = display["resolution"][1].get<int>();
			DisplaySettings_.Fullscreen      = display["fullscreen"].get<bool>();
			const auto &graphics             = settings["graphics"];
			GraphicsSettings_.RenderDistance = graphics["render-distance"].get<int>();
		}
		break;
		default: DCORE_LOG_ERROR << "Bad Preferences file version (" << Version_ << ")"; break;
		}
	}

	static Preferences *prefinst_ = nullptr;
	Preferences *Preferences::Instance()
	{
		if(prefinst_ == nullptr) prefinst_ = new Preferences();
		return prefinst_;
	}

	void Preferences::SetInstance(Preferences *newInstance) { prefinst_ = newInstance; }

	int Preferences::GetVersion() { return Version_; }

	const Preferences::DisplaySettings &Preferences::GetDisplaySettings() const { return DisplaySettings_; }

	const Preferences::GraphicsSettings &Preferences::GetGraphicsSettings() const { return GraphicsSettings_; }
} // namespace dcore
