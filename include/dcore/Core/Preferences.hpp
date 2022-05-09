#pragma once
#include <dcore/Data/FileInput.hpp>
#include <dcore/Data/FileOutput.hpp>
#include <dcore/Launch.hpp>
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
			float FOV;	
		};

		void Read(const dcore::data::FileInput &input);
		void Save(const dcore::data::FileOutput &output);

		int GetVersion() const;
		const DisplaySettings &GetDisplaySettings() const;
		const GraphicsSettings &GetGraphicsSettings() const;

		static Preferences *Instance();

	private:
		friend class launch::Launch;
		static void SetInstance(Preferences *newInstance);

		int Version_;
		DisplaySettings DisplaySettings_;
		GraphicsSettings GraphicsSettings_;
	};
} // namespace dcore
