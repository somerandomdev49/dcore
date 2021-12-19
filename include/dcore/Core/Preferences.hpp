#pragma once
#include <dcore/Data/FileInput.hpp>
#include <dcore/Data/FileOutput.hpp>
#include <glm/glm.hpp>

namespace dcore
{
	class Preferences
	{
	public:
		struct DisplaySettings
		{
			glm::ivec2 Resolution;
			bool Fullscreen;
		};

		struct GraphicsSettings
		{
			int RenderDistance;
		};

		void Read(const dcore::data::FileInput &input);
		void Save(const dcore::data::FileOutput &output);

		int GetVersion();
		const DisplaySettings &GetDisplaySettings() const;
		const GraphicsSettings &GetGraphicsSettings() const;

	private:
		int Version_;
		DisplaySettings DisplaySettings_;
		GraphicsSettings GraphicsSettings_;
	};
} // namespace dcore
