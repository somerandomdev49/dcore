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
		float lastTime = Frame_->GetCurrentTime();
		World_->Start();
		while(!Frame_->ShouldEnd())
		{
			float thisTime = Frame_->GetCurrentTime();
			float dt       = thisTime - lastTime;
			event::TimeManager::Instance()->SetDeltaTime(dt);

			Frame_->OnBeginFrame();
			World_->Update();

			Rend_->OnBeginRender();
			World_->Render(RI_);
			Rend_->OnEndRender();

			Frame_->OnEndFrame();

			lastTime = thisTime;

			FrameLog::Instance()->Flush();
		}
		World_->End();
	}

	Frame *Context::GetFrame() const { return Frame_; }
	dcore::world::World *Context::GetWorld() const { return World_; }

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
