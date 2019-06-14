#pragma once

#include <stdint.h>

namespace Hazel {
	class BufferLayout;
	enum class ShaderDataType;
	size_t SizeOfShaderDataType(ShaderDataType type);

	enum BufferType
	{
		VERTEX, INDEX
	};

	template<typename T, BufferType type>
	class GraphicsBuffer
	{
	public:
		GraphicsBuffer() {}
		virtual void SetData(T* data, uint64_t elements) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t Count() const = 0;
		virtual void SetLayout(BufferLayout& layout) = 0;

		virtual ~GraphicsBuffer() {}
	};


}
