#include <dcore/Platform/Platform.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Resource/Properties.hpp>
#include <dcore/Platform/Window.hpp>

// TODO: Make ethe implementation dynamic.
#include <dcore/Platform/Impl/GLFW/GLFW.hpp>
#include <dcore/Platform/Impl/GLFW/Window.hpp>

using namespace dcore::platform;

void Context::Initialize()
{
    // auto size = resource::Properties::DefaultInstance()->GetIVec2("WindowSize");
    platform::impl::glfw::Initialize();
    Frame_ = new platform::impl::glfw::Frame();
    Frame_->Initialize(glm::ivec2(800, 600));
    Rend_->Initialize();
    RI_ = new graphics::RendererInterface();
}

void Context::DefaultResourceInit(resource::ResourceManager DCORE_REF *rm)
{
    RI_->Initialize(rm, Rend_);
}

dcore::graphics::RendererInterface *Context::GetRendererInterface() const { return RI_; }

void Context::Start()
{
    while(!Frame_->ShouldEnd())
    {
        Frame_->OnBeginFrame();
        // float dt = Frame_->delta(); (void)dt;
        // this->TimeManager_->SetDelta_(dt);
        World_->Update();
        Rend_->OnBeginRender();
        World_->Render(RI_);
        // Rend_->FlushQueue();
        Rend_->OnEndRender();
        Frame_->OnEndFrame();
    }
}

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
void Context::SetInstance(Context *newContext)
{ ctx = newContext; }

Context *Context::Instance()
{ if(ctx == nullptr) ctx = new Context; return ctx; }

bool Context::IsKeyPressed(int key) { return Frame_->CheckKeyPressed(key); }
bool Context::IsMousePressed(int button) { return Frame_->CheckMouseButtonPressed(button); }
