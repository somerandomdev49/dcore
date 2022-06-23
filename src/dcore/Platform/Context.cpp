#include <dcore/Resource/Properties.hpp>
#include <dcore/Event/TimeManager.hpp>
#include <dcore/Platform/Platform.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/Platform/Window.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Core/FrameLog.hpp>

// TODO: Make the implementation dynamic.
#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <dcore/Platform/Impl/GLFW/Window.hpp>
#include <dcore/Platform/Impl/GL3W/GL3W.hpp>
#include <dcore/Core/Preferences.hpp>

namespace dcore::platform
{

	void Context::Initialize()
	{
		// auto size = resource::Properties::DefaultInstance()->GetIVec2("WindowSize");
		platform::impl::glfw::Initialize();
		Frame_ = new platform::impl::glfw::Frame();
		Frame_->Initialize(Preferences::Instance()->GetDisplaySettings().Resolution);
		platform::impl::gl3w::Initialize();
		Rend_->Initialize();
		RI_ = new graphics::RendererInterface();
	}

	void Context::DefaultResourceInit() { RI_->Initialize(resource::ResourceManager::Instance(), Rend_); }

	dcore::graphics::RendererInterface *Context::GetRendererInterface() const { return RI_; }

	void Context::Start()
	{
		Stats_.FPS = 0;
		Stats_.RenderTime = 0;
		Stats_.UpdateTime = 0;
		
		float lastTime = Frame_->GetCurrentTime();
		float fpsTimer = 0;
		int fpsCounter = 0;
		
		World_->Start();
		Frame_->OnBegin();

		while(!Frame_->ShouldEnd())
		{
			float thisTime = Frame_->GetCurrentTime();
			float dt       = thisTime - lastTime;
			event::TimeManager::Instance()->SetDeltaTime(dt);

			fpsTimer += dt;
			if(fpsTimer >= 1)
			{
				Stats_.FPS = fpsCounter;
				fpsCounter = 0;
				fpsTimer = 0;
			}
			
			++fpsCounter;

			float timestamp = 0;

			if(event::InputManager::Instance()->IsMousePressed(0))
			{
				Frame_->SetCursorState(Frame::CursorState_Down);
			}
			else
			{
				Frame_->SetCursorState(Frame::CursorState_Normal);
			}

			timestamp = Frame_->GetCurrentTime();
			Frame_->OnBeginFrame();
			Stats_.FrameBeginTime = Frame_->GetCurrentTime() - timestamp;
			
			timestamp = Frame_->GetCurrentTime();
			World_->Update();
			Stats_.UpdateTime = Frame_->GetCurrentTime() - timestamp;

			timestamp = Frame_->GetCurrentTime();
			Rend_->OnBeginRender();
			World_->Render(RI_);
			Rend_->OnEndRender();
			Stats_.RenderTime = Frame_->GetCurrentTime() - timestamp;

			timestamp = Frame_->GetCurrentTime();
			Frame_->OnEndFrame();
			Stats_.FrameEndTime = Frame_->GetCurrentTime() - timestamp;

			lastTime = thisTime;

			FrameLog::Instance()->Flush();
		}
		World_->End();
	}

	// TODO: Inline into the header
	Frame *Context::GetFrame() const { return Frame_; }

	void Context::CloseWindow() {}

	void Context::DeInitialize()
	{
		RI_->DeInitialize(); // TODO: Move to DefaultResourceDeInit or sth.
		delete RI_;
		Rend_->DeInitialize();
		Frame_->DeInitialize();
		delete Frame_;
	}

	static Context *ctx;
	void Context::SetInstance(Context *newContext) { ctx = newContext; }

	Context *Context::Instance()
	{
		if(ctx == nullptr) ctx = new Context;
		return ctx;
	}

	bool Context::IsKeyPressed(event::KeyCode key) { return Frame_->CheckKeyPressed(key); }
	bool Context::IsMousePressed(int button) { return Frame_->CheckMouseButtonPressed(button); }
} // namespace dcore::platform
