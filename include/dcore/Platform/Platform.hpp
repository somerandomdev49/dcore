#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Event/InputManager.hpp>
#include <dcore/Launch.hpp>
// #include <dcore/Renderer/Renderer.hpp>

namespace dcore::world { class World; }
namespace dcore::graphics { class Renderer; }

namespace dcore::platform
{
	class PlatformSpecific
	{
	public:
		void Initialize();
		void Deinitialize();
	};

	/** Graphics/Window context. Only a single instance should ever exist. */
	class Context
	{
	public:
		static Context *Instance();
	private:
		friend class event::InputManager;
		friend class launch::Launch;
        bool IsMousePressed(int button);
        bool IsKeyPressed(int key);

		static void SetInstance(Context *newContext);
		
		void Initialize();
		void DefaultResourceInit(resource::ResourceManager DCORE_REF *rm);
		void Start();
		void CloseWindow();
		void DeInitialize();

		graphics::Renderer *Rend_;
		world::World *World_;
	};
}
