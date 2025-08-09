#include "Buffer.h"
#include "Vulkan/VulkanBuffer.h"
#include "RendererAPI.h"

VertexBuffer* VertexBuffer::Create()
{
	switch (IRenderer::GetAPI())
	{
	case IRenderer::RendererSelection::Vulkan:
		return new VulkanVertexBuffer();
	default:
		return nullptr;
	}
    return nullptr;
}

IndexBuffer* IndexBuffer::Create()
{
	switch (IRenderer::GetAPI())
	{
	case IRenderer::RendererSelection::Vulkan:
		return new VulkanIndexBuffer();
	default:
		return nullptr;
	}
	return nullptr;
}
