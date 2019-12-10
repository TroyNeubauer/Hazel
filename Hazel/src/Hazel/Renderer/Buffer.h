#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>

#include "Hazel/Core/Core.h"
#include "GraphicsAPI.h"

namespace Hazel {

	enum class GPUPrimitive
	{
		TRIANGLES, LINES
	};

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

	uint32_t SizeOfShaderDataType(ShaderDataType type);

	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement(ShaderDataType type, const char* name, bool normalized = false)
			: Name(name), Type(type), Size(SizeOfShaderDataType(type)), Offset(0), Normalized(normalized) {}

		uint32_t GetElementCount() const;
	};

	class BufferLayout {
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
		BufferLayout() : m_Stride(0) {}

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		inline uint32_t GetStride() const { return m_Stride; }

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	enum class BufferType
	{
		VERTEX, INDEX
	};

	enum class MapAccess
	{
		READ_ONLY, WRITE_ONLY, READ_WRITE
	};

	template<typename T, BufferType type>
	class Buffer
	{
	public:
		Buffer() {}
		inline void SetData(T* data, uint64_t bytes) { SetData(reinterpret_cast<void*>(data), bytes); };
		virtual void SetData(void* data, uint64_t bytes) = 0;
		inline void SetData(std::vector<T>& vec) { SetData(vec.data(), sizeof(T) * vec.size()); }

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void* Map(MapAccess access) = 0;
		virtual void Unmap(void* buffer) = 0;

		//Returns the number of elements in this buffer (usually index count or vertex count)
		inline uint32_t Count() const { return Bytes() / GetLayout().GetStride(); }
		
		virtual uint64_t Bytes() const = 0;

		//Sets this buffer to have the desired layout and uses the structSize parameter (is specified) to verify that the size of the layout
		//requested and the size of the c++ struct maps to that data are the same size
		virtual void SetLayout(const BufferLayout& layout, size_t structSize = 0) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static Ref<Buffer<float, BufferType::VERTEX>> Create(float* data, uint64_t bytes);
		static Ref<Buffer<uint32_t, BufferType::INDEX>> Create(uint32_t* data, uint64_t bytes);

		static Ref<Buffer<float, BufferType::VERTEX>> CreateVertex(uint64_t bytes);
		static Ref<Buffer<uint32_t, BufferType::INDEX>> CreateIndex(uint64_t bytes);

		virtual ~Buffer() {}
	};

	typedef Buffer<float, BufferType::VERTEX> VertexBuffer;
	typedef Buffer<uint16_t, BufferType::INDEX> ShortIndexBuffer;
	typedef Buffer<uint32_t, BufferType::INDEX> IndexBuffer;
}
