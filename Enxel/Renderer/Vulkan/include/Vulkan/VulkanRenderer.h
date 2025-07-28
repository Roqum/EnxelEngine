#pragma once
#include "IRenderer.h"

#include <vulkan/vulkan.h>

class VulkanRenderer : public IRenderer
{
	// Inherited via IRenderer
	void Init() override;
	void RenderFrame() override;
	void Shutdown() override;
};