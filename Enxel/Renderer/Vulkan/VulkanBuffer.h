#pragma once

#include "Buffer.h"
#include <memory>
#include <vulkan/vulkan.h>

namespace Enxel
{

    class VulkanVertexBuffer : public VertexBuffer
    {
    public:

        VulkanVertexBuffer() = default;
        VulkanVertexBuffer(VkDeviceSize size, VkDevice* device, VkBuffer buffer, VkDeviceMemory deviceMemory);
        virtual ~VulkanVertexBuffer() {}

        virtual void Bind() const override;
        virtual void Unbind() const override;

        inline virtual void SetLayout(const VertexBufferLayout& bufferLayout) { m_Layout = bufferLayout;}
        inline virtual const VertexBufferLayout& GetLayout() const { return m_Layout; }

	    VkBuffer m_VkBuffer;
	    VkDeviceMemory m_VkBufferMemory;
        std::unique_ptr<VkDevice> m_Vkdevice;
	    VkDeviceSize m_Size;
    private:

        VertexBufferLayout m_Layout = {};
    };

    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        VulkanIndexBuffer() = default;
        VulkanIndexBuffer(VkDeviceSize size, VkDevice* device, VkBuffer buffer, VkDeviceMemory deviceMemory);
        virtual ~VulkanIndexBuffer() {}

        virtual void Bind() const override;
        virtual void Unbind() const override;

        VkBuffer m_VkBuffer;
        VkDeviceMemory m_VkBufferMemory;
        std::unique_ptr<VkDevice> m_Vkdevice;
        VkDeviceSize m_Size;
    private:
    };
}