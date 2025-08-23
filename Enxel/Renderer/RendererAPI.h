#pragma once
#include <SDL3/SDL_video.h>
#include "Buffer.h"
#include <memory>
#include "imgui.h"
namespace Enxel
{

    class IRenderer {
    public:
        enum class RendererSelection
        {
            None = 0, 
            Vulkan = 1, 
            DirectX12 = 2
        };

        virtual ~IRenderer() = default;

        static IRenderer* Create();

        virtual void Initialize(SDL_Window* sdlWindow, ImGuiContext* imguiContext) = 0;
        virtual void BeginScene() = 0;
        virtual void EndScene() = 0;
        virtual void Submit(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) = 0;
        virtual void RenderFrame() = 0;
        virtual void Shutdown() = 0;
        virtual void StopRendering() = 0;
#ifdef PROFILING
        virtual double GetGPUCycleDuration() = 0;
#endif


        virtual VertexBuffer* CreateVertexBuffer(std::vector<Vertex>& verticies) = 0;
        virtual IndexBuffer* CreateIndexBuffer(std::vector<uint32_t>& indices) = 0;
    
        static RendererSelection GetAPI() { return s_RendererSelection; }

    private:
	
        static RendererSelection s_RendererSelection;

 
    };
}