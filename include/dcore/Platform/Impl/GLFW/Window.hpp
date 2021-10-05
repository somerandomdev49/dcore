#pragma once
#include <dcore/Platform/Window.hpp>
#include <GLFW/glfw3.h>

namespace dcore::platform::impl::glfw
{
	class Frame : dcore::platform::Frame
	{
	public:
		void Initialize(const glm::ivec2 &size) override;
		bool ShouldEnd() override;
		void OnBeginFrame() override;
		void OnEndFrame() override;
		void DeInitialize() override;

		bool CheckKeyPressed(int key) override;
		bool CheckMouseButtonPressed(int button) override;
	private:
		GLFWwindow *Window_;
	};
}
