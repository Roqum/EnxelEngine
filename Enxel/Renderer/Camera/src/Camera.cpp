#include "BufferLayout.h"

VertexBufferLayout::VertexBufferLayout(std::initializer_list<BufferElement> bufferElements) :
	m_BufferElements(bufferElements)
{
	CalculateOffsetAndStride();
}

void VertexBufferLayout::CalculateOffsetAndStride()
{
	uint32_t offset = 0;
	uint32_t stride = 0;
	for (BufferElement& element : m_BufferElements)
	{
		element.Offset = offset;
		offset += element.Size;
		m_Stride += element.Size;
	}
}