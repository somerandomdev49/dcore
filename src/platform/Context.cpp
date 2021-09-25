#include <dcore/Platform/Platform.hpp>
#include <dcore/Resource/Properties.hpp>
#include <fwdraw.hpp>

using namespace dcore::platform;

static fwdraw::Frame *frame;

void Context::Initialize()
{
    // auto size = resource::Properties::DefaultInstance()->GetIVec2("WindowSize");
    frame = new fwdraw::Frame(glm::ivec2(800, 600));
    frame->init();
}

void Context::OpenWindow()
{
    // ..?
}

void Context::Start()
{
    while(!frame->should_end())
    {
        frame->on_begin();
        float dt = frame->delta();
        // this->TimeManager_->SetDelta_(dt);
        Rend_->OnBeginRender();
        Rend_->FlushQueue();
        Rend_->OnEndRender();
        frame->on_end();
    }
}

void Context::CloseWindow() {}

void Context::DeInitialize() {}

static Context *ctx;
void Context::SetInstance(Context *newContext)
{ ctx = newContext; }

Context *Context::Instance()
{ if(ctx == nullptr) ctx = new Context; return ctx; }
