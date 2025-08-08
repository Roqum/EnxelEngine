#pragma once
#include <SDL3/SDL_video.h>

class IRenderer {
public:
    enum class RendererSelection
    {
        None = 0, 
        Vulkan = 1, 
        DirectX12 = 2
    };

    virtual ~IRenderer() = default;

    static IRenderer* CreateRenderer();
    virtual void Initialize(SDL_Window* sdlWindow) = 0;
    virtual void RenderFrame() = 0;
    virtual void Shutdown() = 0;
    virtual void StopRendering() = 0;

    static RendererSelection GetAPI() { return s_RendererSelection; }

private:
	
    static RendererSelection s_RendererSelection;
};