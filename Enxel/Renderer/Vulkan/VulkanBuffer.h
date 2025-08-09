#pragma once
#include "Buffer.h"


class VulkanVertexBuffer : public VertexBuffer
{
public:

    VulkanVertexBuffer();
    virtual ~VulkanVertexBuffer() {}

    virtual void Bind() const override;
    virtual void Unbind() const override;

    inline virtual void SetLayout(const VertexBufferLayout& bufferLayout) { m_Layout = bufferLayout;}
    inline virtual const VertexBufferLayout& GetLayout() const { return m_Layout; }

private:
    VertexBufferLayout m_Layout;
};

class VulkanIndexBuffer : public IndexBuffer
{
public:
    VulkanIndexBuffer();
    virtual ~VulkanIndexBuffer() {}

    virtual void Bind() const override;
    virtual void Unbind() const override;

};