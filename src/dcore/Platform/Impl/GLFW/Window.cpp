#include <dcore/Platform/Impl/GLFW/Window.hpp>
#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <dcore/Core/Log.hpp>
#include <dcore/Core/Application.hpp>
#include <glm/glm.hpp>
#include <imgui_impl_glfw.h>

namespace dcore::platform::impl
{
	glfw::Frame::Frame() = default;
	glfw::Frame::~Frame() = default;

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
			
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		io.ConfigViewportsNoTaskBarIcon = true;

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

	void glfw::Frame::OnBeginFrame() {}
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
		glfwPollEvents();
	}

	float glfw::Frame::GetCurrentTime() { return (float)glfwGetTime(); }

	bool glfw::Frame::CheckKeyPressed(event::KeyCode key) { return glfwGetKey(Window_, (int)key) == GLFW_PRESS; }
	bool glfw::Frame::CheckMouseButtonPressed(int button) { return glfwGetMouseButton(Window_, button) == GLFW_PRESS; }
} // namespace dcore::platform::impl
