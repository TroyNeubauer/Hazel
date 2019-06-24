#pragma once
#include <vector>

#include <glad/glad.h>

#include "GraphicsAPI.h"

namespace Hazel {

	enum class ShaderDataType {
		None = 0,
		Float16, Float16_2, Float16_3, Float16_4, 
		Float32, Float32_2, Float32_3, Float32_4,
		Float64, Float64_2, Float64_3, Float64_4,
		
		Int8, Int8_2, Int8_3, Int8_4,
		Int16, Int16_2, Int16_3, Int16_4,
		Int32, Int32_2, Int32_3, Int32_4,
		Int64, Int64_2, Int64_3, Int64_4,
		
		Mat3, Mat4, Bool,
		//Constants for convience
		Int = Int32, Int2 = Int32_2, Int3 = Int32_3, Int4 = Int32_4,
		Float = Float32, Float2 = Float32_2, Float3 = Float32_3, Float4 = Float32_4,
	};

	size_t SizeOfShaderDataType(ShaderDataType type);

	struct BufferElement {
		const char* Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement(ShaderDataType type, const char* name, bool normalized = false)
			: Name(name), Type(type), Size(SizeOfShaderDataType(type)), Offset(0), Normalized(normalized) {}

		BufferElement() {}

		uint32_t GetElementCount() const;
	};

	class BufferLayout {
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			for (auto& element : m_Elements) {
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
		BufferLayout() {}

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		inline uint32_t GetStride() const { return m_Stride; }

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	enum BufferType
	{
		VERTEX, INDEX
	};

	template<typename T, BufferType type>
	class Buffer
	{
	public:
		Buffer() {}
		virtual void SetData(T* data, uint64_t elements) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t Count() const = 0;
		inline size_t ElementSize() const { return sizeof(T); }
		inline size_t Bytes() const { return ElementSize() * Count(); }

		virtual void SetLayout(BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Buffer<float, BufferType::VERTEX>* Create(float* data, uint64_t elements);
		static Buffer<uint32_t, BufferType::INDEX>* Create(uint32_t* data, uint64_t elements);

		virtual ~Buffer() {}
	};

	typedef Buffer<float, BufferType::VERTEX> VertexBuffer;
	typedef Buffer<uint16_t, BufferType::INDEX> ShortIndexBuffer;
	typedef Buffer<uint32_t, BufferType::INDEX> IndexBuffer;


}
