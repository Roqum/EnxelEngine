#pragma once

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void Init() = 0;
    virtual void RenderFrame() = 0;
    virtual void Shutdown() = 0;
};