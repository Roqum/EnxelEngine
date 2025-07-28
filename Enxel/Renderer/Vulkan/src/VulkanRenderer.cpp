#include "VulkanRenderer.h"
#include <iosteam>

extern "C" __declspec(dllexport) IRenderer* CreateRenderer() {
    return new VulkanRenderer();
}

extern "C" __declspec(dllexport) void DestroyRenderer(IRenderer* renderer) {
    delete renderer;
}

void VulkanRenderer::Init()
{
	std::cout << "VulkanRenderer initialized." << std::endl;
}

void VulkanRenderer::RenderFrame()
{
}

void VulkanRenderer::Shutdown()
{
}
