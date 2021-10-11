#include <dcore/Platform/Impl/GL3W/GL3W.hpp>
#include <dcore/Core/Application.hpp>
#include <GL/gl3w.h>

using namespace dcore::platform::impl;

bool gl3w::DidInitialize = false;
bool gl3w::DidDeInitialize = false;

void gl3w::Initialize()
{
	if(gl3wInit())
	{
		Application::Info.SetError(true, "Could not initialize OpenGL context!", "Check if your drivers are up to date");
		return;
	}

	if(!gl3wIsSupported(4, 1))
	{
		Application::Info.SetError(true, "OpenGL 4.1 is not supported!", "Check if your drivers are up to date");
		return;
	}

	DidInitialize = true;
}

void gl3w::DeInitialize() { if(DidInitialize) gl3wClose(); DidDeInitialize = true; }