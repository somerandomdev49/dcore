#pragma once
#include <dcore/Platform/Window.hpp>
#include <GLFW/glfw3.h>

namespace dcore::platform::impl::glfw
{
	class Frame : public dcore::platform::Frame
	{
	public:
		Frame();
		~Frame();

		void  Initialize(const glm::ivec2 &size) override;
		bool  ShouldEnd() override;
		void  OnBeginFrame() override;
		float GetCurrentTime() override;
		void  OnEndFrame() override;
		void  DeInitialize() override;

		bool CheckKeyPressed(event::KeyCode key) override;
		bool CheckMouseButtonPressed(int button) override;

	private:
		GLFWwindow *Window_;
	};
} // namespace dcore::platform::impl::glfw
