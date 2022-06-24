#include <GLFW/glfw3.h>
#include <cstring>
#include <dcore/Platform/Impl/GLFW/Window.hpp>
#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/Application.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Util/LoaderUtil.hpp>

#include <glm/glm.hpp>
#include <imgui_impl_glfw.h>

namespace dcore::platform
{
	void Frame::RCursor_Constructor(const std::string &path, void *placement)
	{
		auto *cursor = new(placement) RCursor();

		util::ImageData d;
		util::LoaderUtil::LoadImage(d, path);

		if(d.data == nullptr)
		{
			DCORE_LOG_ERROR << "Failed to load cursor at '" << path << "'";
			return;
		}

		auto *img = (GLFWimage*&)cursor->image = new GLFWimage;
		img->width = d.size.x;
		img->height = d.size.y;
		img->pixels = (dstd::Byte*)d.data;
		cursor->cursor = glfwCreateCursor(img, 0, 0);

		// free(d.data); TODO: find out if glfw copies image data
	}

	void Frame::RCursor_DeConstructor(void *placement)
	{
		auto *cursor = (RCursor*)placement;
		glfwDestroyCursor((GLFWcursor*)cursor->cursor);
		free(((GLFWimage*)cursor->image)->pixels);
	}
}

namespace dcore::platform::impl
{
	glfw::Frame::Frame() = default;
	glfw::Frame::~Frame() = default;

	void glfw::Frame::Initialize(const glm::ivec2 &size)
	{
		memset(CursorMap_, 0, sizeof(CursorState) * CursorState_Max_);
		Size_ = size; // TODO: Title
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__ 
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		Window_ = glfwCreateWindow(Size_.x, Size_.y, "DragonCore", nullptr, nullptr);

		if(Window_ == nullptr)
		{
			DCORE_LOG_ERROR << "Failed to create window! Reason: " << glfw::GetError();
			Application::Info.SetError(true, "Failed to create window.", glfw::GetError());
			return;
		}

		glfwHideWindow(Window_);
		glfwMakeContextCurrent(Window_);

		glfwSetWindowUserPointer(Window_, this);

		glfwSetScrollCallback(Window_, [](GLFWwindow *window, double x, double y)
		{
			event::InputManager::Instance()->SetScroll({ x, y });
		});

		glfwSetCursorPosCallback(Window_, [](GLFWwindow *window, double x, double y)
		{
			event::InputManager::Instance()->SetMousePosition({ x, y });
		});
			
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		io.ConfigViewportsNoTaskBarIcon = false;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(Window_, true);
	}

	void glfw::Frame::DeInitialize()
	{
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(Window_);
		Window_ = nullptr;
	}

	bool glfw::Frame::ShouldEnd() { return glfwWindowShouldClose(Window_) != 0; }

	void glfw::Frame::OnBegin() { glfwShowWindow(Window_); }

	void glfw::Frame::OnBeginFrame()
	{
		glfwPollEvents();
	}
	
	void glfw::Frame::OnEndFrame()
	{
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

		glfwSwapBuffers(Window_);

		event::InputManager::Instance()->SetScroll({ 0, 0 });
	}

	float glfw::Frame::GetCurrentTime() { return (float)glfwGetTime(); }

	bool glfw::Frame::CheckKeyPressed(event::KeyCode key) { return glfwGetKey(Window_, (int)key) == GLFW_PRESS; }
	bool glfw::Frame::CheckMouseButtonPressed(int button) { return glfwGetMouseButton(Window_, button) == GLFW_PRESS; }

	void glfw::Frame::SetCursorState(CursorState newState)
	{
		glfwSetCursor(Window_, (GLFWcursor*)CursorMap_[newState]);
	}


	glm::ivec2 glfw::Frame::GetFramebufferSize()
	{
		glm::ivec2 s;
		glfwGetFramebufferSize(Window_, &s.x, &s.y);
		return s;
	}

	void glfw::Frame::SetSize(glm::ivec2 newSize) { Size_ = newSize; }

} // namespace dcore::platform::impl
