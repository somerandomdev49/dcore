#include <dcore/Platform/Impl/GLFW/Window.hpp>
#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/Application.hpp>
#include <glm/glm.hpp>

using namespace dcore::platform::impl;

void glfw::Frame::Initialize(const glm::ivec2 &size)
{
	Size_ = size; // TODO: Title
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	Window_ = glfwCreateWindow(Size_.x, Size_.y, "DragonCore", NULL, NULL);
	if(!Window_)
	{
		DCORE_LOG_ERROR << "Failed to create window! Reason: " << glfw::GetError();
		Application::Info.SetError(true, "Failed to create window.", glfw::GetError());
		return;
	}

	glfwMakeContextCurrent(Window_);
}
