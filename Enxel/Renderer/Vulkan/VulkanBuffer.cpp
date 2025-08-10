#include "VulkanBuffer.h"
#include <vulkan/vulkan.h>

VulkanVertexBuffer::VulkanVertexBuffer(std::vector<Vertex>& vertices) : m_Layout({})
{
    VkDeviceSize bufferSize = vertices.size() * sizeof(Vertex);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    void* data;
    // staging buffer
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingMemory;
    CreateBuffer(size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingMemory);

    // Map & copy in a type-safe way
    void* mapped;
    vkMapMemory(device, stagingMemory, 0, size, 0, &mapped);
    std::memcpy(mapped, vertices.data(), size);
    vkUnmapMemory(device, stagingMemory);

    // GPU-local buffer
    CreateBuffer(size,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        m_Buffer, m_BufferMemory);

    CopyBuffer(stagingBuffer, m_Buffer, size);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingMemory, nullptr);
}

void VulkanVertexBuffer::Bind() const
{

}

void VulkanVertexBuffer::Unbind() const
{
}

VulkanIndexBuffer::VulkanIndexBuffer()
{
}

void VulkanIndexBuffer::Bind() const
{
}

void VulkanIndexBuffer::Unbind() const
{
}
