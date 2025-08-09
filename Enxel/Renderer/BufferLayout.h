#pragma once 

#include <vector>
#include <string>
#include <initializer_list>

enum ShaderDataType
{
	None = 0,
	Float, Float2, Float3, Float4,
	Mat3, Mat4,
	Int, Int2, Int3, Int4,
	Bool
};

static uint32_t GetShaderDataTypeStride(ShaderDataType dataType)
{
	switch (dataType)
	{
	case ShaderDataType::Float:   return 4;
	case ShaderDataType::Float2:  return 4 * 2;
	case ShaderDataType::Float3:  return 4 * 3;
	case ShaderDataType::Float4:  return 4 * 4;
	case ShaderDataType::Mat3:    return 4 * 3 * 3; // 3x3 matrix
	case ShaderDataType::Mat4:    return 4 * 4 * 4; // 4x4 matrix
	case ShaderDataType::Int:     return 4;
	case ShaderDataType::Int2:    return 4 * 2;
	case ShaderDataType::Int3:    return 4 * 3;
	case ShaderDataType::Int4:    return 4 * 4;
	case ShaderDataType::Bool:    return sizeof(bool);
	default:
		// TODO :Assert and log error here (When I have a logger)
		return 0;
	}
}

struct BufferElement
{
	std::string SemanticName;
	ShaderDataType Type;
	uint32_t Size;
	uint32_t Offset;

	BufferElement(ShaderDataType type, const std::string& semanticName)
		: SemanticName(semanticName), Type(type), Size(GetShaderDataTypeStride(type)), Offset(0) {
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout(std::initializer_list<BufferElement> bufferElements);
	virtual ~VertexBufferLayout() {}

	inline std::vector<BufferElement> GetElements() const { return m_BufferElements; }

private:
	std::vector<BufferElement> m_BufferElements;
	uint32_t m_Stride = 0;

	void CalculateOffsetAndStride();
};