#include "application.hpp"
#include <fwdraw.hpp>

Application::Application(Program *program)
    : program(program) {}

void Application::Start()
{
    fwdraw::Frame frame(program->GetSize());
    renderer.Init();
    frame.init();
    program->OnStart();
    while(!frame.should_end())
    {
        renderer.BeginFrame();
        frame.on_begin();
        program->OnTick(frame.delta());
        renderer.Flush();
        frame.on_end();
    }
    program->OnEnd();
    renderer.Deinit();
    frame.deinit();
}   
