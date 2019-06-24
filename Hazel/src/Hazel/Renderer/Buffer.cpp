#include "hzpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/NoAPI/NoAPI.h"

namespace Hazel {
	size_t SizeOfShaderDataType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None:		return 0;
			case ShaderDataType::Float16:	return 2 * 1;
			case ShaderDataType::Float16_2:	return 2 * 2;
			case ShaderDataType::Float16_3:	return 2 * 3;
			case ShaderDataType::Float16_4:	return 2 * 4;
		
			case ShaderDataType::Float32:	return 4 * 1;
			case ShaderDataType::Float32_2:	return 4 * 2;
			case ShaderDataType::Float32_3:	return 4 * 3;
			case ShaderDataType::Float32_4:	return 4 * 4;
		
			case ShaderDataType::Float64:	return 8 * 1;
			case ShaderDataType::Float64_2:	return 8 * 2;
			case ShaderDataType::Float64_3:	return 8 * 3;
			case ShaderDataType::Float64_4:	return 8 * 4;

			case ShaderDataType::Int8:		return 1 * 1;
			case ShaderDataType::Int8_2:	return 1 * 2;
			case ShaderDataType::Int8_3:	return 1 * 3;
			case ShaderDataType::Int8_4:	return 1 * 4;
		
			case ShaderDataType::Int16:		return 2 * 1;
			case ShaderDataType::Int16_2:	return 2 * 2;
			case ShaderDataType::Int16_3:	return 2 * 3;
			case ShaderDataType::Int16_4:	return 2 * 4;
		
			case ShaderDataType::Int32:		return 4 * 1;
			case ShaderDataType::Int32_2:	return 4 * 2;
			case ShaderDataType::Int32_3:	return 4 * 3;
			case ShaderDataType::Int32_4:	return 4 * 4;
		
			case ShaderDataType::Int64:		return 8 * 1;
			case ShaderDataType::Int64_2:	return 8 * 2;
			case ShaderDataType::Int64_3:	return 8 * 3;
			case ShaderDataType::Int64_4:	return 8 * 4;
		
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Bool:		return 1;
			default:
				HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
		return 0;
	}


	VertexBuffer* Buffer<float, BufferType::VERTEX>::Create(float* data, uint64_t elements) {
		switch (GraphicsAPI::Get())
		{
			case GraphicsAPIType::OPEN_GL:	return new OpenGLBuffer<float, BufferType::VERTEX>(data, elements);
			case GraphicsAPIType::NONE:		return new NoAPIBuffer <float, BufferType::VERTEX>(data, elements);
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}

	IndexBuffer* Buffer<uint32_t, BufferType::INDEX>::Create(uint32_t* data, uint64_t elements) {
		switch (GraphicsAPI::Get())
		{
			case GraphicsAPIType::OPEN_GL:	return new OpenGLBuffer<uint32_t, BufferType::INDEX>(data, elements);
			case GraphicsAPIType::NONE:		return new NoAPIBuffer <uint32_t, BufferType::INDEX>(data, elements);
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}


	uint32_t BufferElement::GetElementCount() const
	{
		switch (Type)
		{
			case ShaderDataType::None:		return 0;
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Float16:	return 1;
			case ShaderDataType::Float16_2:	return 2;
			case ShaderDataType::Float16_3:	return 3;
			case ShaderDataType::Float16_4:	return 4;
			case ShaderDataType::Float64:	return 1;
			case ShaderDataType::Float64_2:	return 2;
			case ShaderDataType::Float64_3:	return 3;
			case ShaderDataType::Float64_4:	return 4;
			case ShaderDataType::Int64:		return 1;
			case ShaderDataType::Int64_2:	return 2;
			case ShaderDataType::Int64_3:	return 3;
			case ShaderDataType::Int64_4:	return 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Int16:		return 1;
			case ShaderDataType::Int16_2:	return 2;
			case ShaderDataType::Int16_3:	return 3;
			case ShaderDataType::Int16_4:	return 4;
			case ShaderDataType::Int8:		return 1;
			case ShaderDataType::Int8_2:	return 2;
			case ShaderDataType::Int8_3:	return 3;
			case ShaderDataType::Int8_4:	return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
			default:
				HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
		return 0;
	}

}

