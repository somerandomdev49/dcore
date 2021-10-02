#include <dcore/Platform/Platform.hpp>
#include <dcore/Renderer/Renderer.hpp>
#include <dcore/Graphics/Graphics.hpp>
#include <dcore/World/World.hpp>
#include <dcore/Resource/Properties.hpp>
#include <fwdraw.hpp>

using namespace dcore::platform;

static fwdraw::Frame *frame;
static dcore::graphics::RendererInterface ri;

void Context::Initialize()
{
    // auto size = resource::Properties::DefaultInstance()->GetIVec2("WindowSize");
    frame = new fwdraw::Frame(glm::ivec2(800, 600));
    frame->init();

    Rend_->Initialize();
}

void Context::DefaultResourceInit(resource::ResourceManager DCORE_REF *rm)
{
    ri.Initialize(rm, Rend_);
}

dcore::graphics::RendererInterface *Context::GetRendererInterface() const { return &ri; }

void Context::Start()
{
    while(!frame->should_end())
    {
        frame->on_begin();
        float dt = frame->delta(); (void)dt;
        // this->TimeManager_->SetDelta_(dt);
        World_->Update();
        Rend_->OnBeginRender();
        World_->Render(&ri);
        // Rend_->FlushQueue();
        Rend_->OnEndRender();
        frame->on_end();
    }
}

void Context::CloseWindow() {}

void Context::DeInitialize()
{
    ri.DeInitialize(); // TODO: Move to DefaultResourceDeInit or sth.
    Rend_->DeInitialize();
    frame->deinit();
}

static Context *ctx;
void Context::SetInstance(Context *newContext)
{ ctx = newContext; }

Context *Context::Instance()
{ if(ctx == nullptr) ctx = new Context; return ctx; }

bool Context::IsKeyPressed(int key) { return frame->key_pressed(key); }
bool Context::IsMousePressed(int button) { return frame->mouse_pressed(button); }
