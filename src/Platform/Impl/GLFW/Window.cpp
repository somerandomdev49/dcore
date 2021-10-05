#include <dcore/Platform/Impl/GLFW/Window.hpp>
#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <glm/glm.hpp>
#include <dcore/Core/Log.hpp>

using namespace dcore::platform::impl;

void glfw::Frame::Initialize(const glm::ivec2 &size)
{

	Size_ = size;
	Window_ = glfwCreateWindow(Size_.x, Size_.y, "DragonCore", NULL, NULL);
	if(!Window_)
	{
		const char *err;
		glfwGetError(&err);
		DCORE_LOG_ERROR << "Failed to create window! Reason: " << err;
		return;
	}


}
