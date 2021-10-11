#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <dcore/Core/Log.hpp>
#include <GLFW/glfw3.h>

using namespace dcore::platform::impl;

bool glfw::DidInitialize = false;
bool glfw::DidTerminate  = false;

void glfw::Initialize() {
	if(!glfw::DidInitialize) {
		if(!glfwInit()) {
			const char *err;
			glfwGetError(&err);
			DCORE_LOG_ERROR << "Failed to initialize GLFW! Reason: " << err;
			return;
		}

		glfw::DidInitialize = true;
	}
}

void glfw::DeInitilize() {
	if(!DidTerminate) {
		glfwTerminate();
		DidTerminate = true;
	}
}

const char *glfw::GetError() {
	const char *err;
	glfwGetError(&err);
	return err;
}
