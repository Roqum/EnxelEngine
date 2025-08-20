#include "VulkanBuffer.h"
#include <vulkan/vulkan.h>

namespace Enxel
{

    VulkanVertexBuffer::VulkanVertexBuffer(VkDeviceSize size, VkDevice* device, VkBuffer buffer, VkDeviceMemory deviceMemory)
        : m_Size(size), m_Vkdevice(device), m_Layout({}), m_VkBuffer(buffer), m_VkBufferMemory(deviceMemory)
    {
    
    }

    void VulkanVertexBuffer::Bind() const
    {

    }

    void VulkanVertexBuffer::Unbind() const
    {
        vkDestroyBuffer(*m_Vkdevice, m_VkBuffer, nullptr);
        vkFreeMemory(*m_Vkdevice, m_VkBufferMemory, nullptr);
    }

    VulkanIndexBuffer::VulkanIndexBuffer(VkDeviceSize size, VkDevice* device, VkBuffer buffer, VkDeviceMemory deviceMemory)
	    : m_Size(size), m_Vkdevice(device), m_VkBuffer(buffer), m_VkBufferMemory(deviceMemory)
    {
    }

    void VulkanIndexBuffer::Bind() const
    {
    }

    void VulkanIndexBuffer::Unbind() const
    {
        vkDestroyBuffer(*m_Vkdevice, m_VkBuffer, nullptr);
        vkFreeMemory(*m_Vkdevice, m_VkBufferMemory, nullptr);
    }

}