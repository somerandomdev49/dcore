#pragma once

namespace dcore::graphics
{
    class RSkyBox
    {
        friend class Renderer;
        friend class RenderResourceManager;
        void *Data_;
    };
}
