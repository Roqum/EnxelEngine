#pragma once
#include <SDL3/SDL_video.h>

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual void CreateRenderer(SDL_Window* sdlWindow) = 0;
    virtual void RenderFrame() = 0;
    virtual void Shutdown() = 0;
    virtual void StopRendering() = 0;
};