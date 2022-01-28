#include <dcore/Platform/Impl/GLFW/Window.hpp>
#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/Application.hpp>
#include <glm/glm.hpp>

namespace dcore::platform::impl
{

	glfw::Frame::Frame() {}
	glfw::Frame::~Frame() {}

	void glfw::Frame::Initialize(const glm::ivec2 &size)
	{
		Size_ = size; // TODO: Title
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		// TODO: Check if using OpenGL.
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// #ifdef __darwin__ // FIXME: correct name
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		// #endif

		Window_ = glfwCreateWindow(Size_.x, Size_.y, "DragonCore", NULL, NULL);

		if(!Window_)
		{
			DCORE_LOG_ERROR << "Failed to create window! Reason: " << glfw::GetError();
			Application::Info.SetError(true, "Failed to create window.", glfw::GetError());
			return;
		}

		glfwHideWindow(Window_);
		glfwMakeContextCurrent(Window_);
	}

	void glfw::Frame::DeInitialize()
	{
		glfwDestroyWindow(Window_);
		Window_ = nullptr;
	}

	bool glfw::Frame::ShouldEnd() { return glfwWindowShouldClose(Window_); }

	void glfw::Frame::OnBegin() { glfwShowWindow(Window_); }

	void glfw::Frame::OnBeginFrame() {}
	void glfw::Frame::OnEndFrame()
	{
		glfwSwapBuffers(Window_);
		glfwPollEvents();
	}

	float glfw::Frame::GetCurrentTime() { return (float)glfwGetTime(); }

	bool glfw::Frame::CheckKeyPressed(event::KeyCode key) { return glfwGetKey(Window_, (int)key) == GLFW_PRESS; }
	bool glfw::Frame::CheckMouseButtonPressed(int button) { return glfwGetMouseButton(Window_, button) == GLFW_PRESS; }
} // namespace dcore::platform::impl
