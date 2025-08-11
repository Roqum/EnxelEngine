#include "RendererAPI.h"
#include "Vulkan/VulkanRenderer.h"

namespace Enxel
{ 
IRenderer::RendererSelection IRenderer::s_RendererSelection  = IRenderer::RendererSelection::Vulkan;
    
    IRenderer* IRenderer::Create()
    {
        if (s_RendererSelection == RendererSelection::Vulkan)
        {
            return new VulkanRenderer();
        }
        else
        {
		    assert(false && "Renderer API not supported!");
        }
    }
}

