#pragma once
#include <dcore/Resource/ResourceManager.hpp>
#include <dcore/Event/InputManager.hpp>
#include <dcore/Platform/Window.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Launch.hpp>
// #include <dcore/Renderer/Renderer.hpp>

namespace dcore::world
{
	class World;
}

namespace dcore::graphics
{
	class Renderer;
}

namespace dcore::platform
{
	class PlatformSpecific
	{
	public:
		void Initialize();
		void Deinitialize();
	};

	/** Graphics/Window context. */
	class Context
	{
	public:
		dcore::graphics::RendererInterface *GetRendererInterface() const;
		Frame *GetFrame() const;
		static Context *Instance();

		/**
		 * @brief Returns the current world instance.
		 * 
		 * @return The current world instance.
		 */
		world::World *GetWorld() const { return World_; }

		/**
		 * @brief Sets the current world instance
		 * @note Preferably used only by the world loader.
		 * 
		 * @param world The new world instance
		 */
		void SetWorld(world::World *newWorld) { World_ = newWorld; };

	private:
		friend class event::InputManager;
		friend class launch::Launch;

		bool IsKeyPressed(event::KeyCode key);
		bool IsMousePressed(int button);

		static void SetInstance(Context *newContext);

		void Initialize();
		void DefaultResourceInit();
		void Start();
		void CloseWindow();
		void DeInitialize();

		graphics::Renderer *Rend_;
		world::World *World_;
		Frame *Frame_;
		dcore::graphics::RendererInterface *RI_;
	};
} // namespace dcore::platform
