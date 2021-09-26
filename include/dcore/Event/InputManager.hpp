#pragma once

namespace dcore::platform { class Context; }

namespace dcore::event
{
    class InputManager
    {
    public:
        void Initialize();
        void DeInitialize();
        bool IsMousePressed(int button);
        bool IsKeyPressed(int key);
    private:
        friend class platform::Context;
    };
}
