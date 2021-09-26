#pragma once
#include <dcore/Event/InputManager.hpp>
#include <dcore/Graphics/Graphics.hpp>

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
		void Initialize();
		void OpenWindow();
		void Start();
		void CloseWindow();
		void DeInitialize();

		static void SetInstance(Context *newContext);
		static Context *Instance();
	private:
		friend class event::InputManager;
        bool IsMousePressed(int button);
        bool IsKeyPressed(int key);

		graphics::Renderer *Rend_;
	};
}
