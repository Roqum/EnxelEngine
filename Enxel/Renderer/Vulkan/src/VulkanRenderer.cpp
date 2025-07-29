#include "Vulkan/VulkanRenderer.h"
#include <iostream>

#ifdef _WIN32
extern "C" __declspec(dllexport) IRenderer* CreateRenderer() {
    return new VulkanRenderer();
}

extern "C" __declspec(dllexport) void DestroyRenderer(IRenderer* renderer) {
    delete renderer;
}
#elif
extern "C" IRenderer* CreateRenderer() {
    return new VulkanRenderer();
}

extern "C" void DestroyRenderer(IRenderer* renderer) {
    delete renderer;
}
#endif
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
