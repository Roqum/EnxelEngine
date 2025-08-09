#pragma once
#include "Buffer.h"


class VulkanVertexBuffer : public VertexBuffer
{
public:

    VulkanVertexBuffer();
    virtual ~VulkanVertexBuffer() {}

    virtual void Bind() const override;
    virtual void Unbind() const override;

};

class VulkanIndexBuffer : public IndexBuffer
{
public:
    VulkanIndexBuffer();
    virtual ~VulkanIndexBuffer() {}

    virtual void Bind() const override;
    virtual void Unbind() const override;

};