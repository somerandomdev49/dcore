#pragma once
#include "renderer.hpp"
#include <glm/glm.hpp>

class Application;
class Program
{
public:
    virtual glm::ivec2 GetSize() = 0;

    virtual void OnStart() = 0;
    virtual void OnTick(float deltaTime) = 0;
    virtual void OnEnd() = 0;

private:
    friend class Application;
    Application *app_;
};

class Application
{
public:
    Application(Program *program);
    void Start();
private:
    Renderer renderer;
    Program *program;
};
