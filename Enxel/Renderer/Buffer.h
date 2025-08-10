#pragma once
#include "BufferLayout.h"
#include "Vulkan/Vertex.h"

class VertexBuffer
{
public:
    static VertexBuffer* Create(std::vector<Vertex>& vertices);
    virtual ~VertexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetLayout(const VertexBufferLayout& bufferLayout) = 0;
    virtual const VertexBufferLayout& GetLayout() const = 0;


};

class IndexBuffer
{
public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static IndexBuffer* Create();
};



