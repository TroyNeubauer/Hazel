#pragma once
#include <vector>

#include <glad/glad.h>

#include "GraphicsAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {

	enum class ShaderDataType {
		None = 0,
		Float, Float2, Float3, Float4, 
		Float16, Float16_2, Float16_3, Float16_4, 
		Float64, Float64_2, Float64_3, Float64_4,
		Int16, Int16_2, Int16_3, Int16_4,
		Int, Int2, Int3, Int4,
		Int64, Int64_2, Int64_3, Int64_4,
		Int8, Int8_2, Int8_3, Int8_4,
		Mat3, Mat4, Bool
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

		uint32_t GetElementCount() const
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


	typedef GraphicsBuffer<float, BufferType::VERTEX> VertexBuffer;
	typedef GraphicsBuffer<uint32_t, BufferType::INDEX> IndexBuffer;

	class Buffer {
	public:

		template<typename T, BufferType type>
		static GraphicsBuffer<T, type>* Create(T* data, uint64_t elements) {

			if (GraphicsAPI::Get() == GraphicsAPI::OPEN_GL)
				return new OpenGLBuffer<T, type>(data, elements);
			else HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");

			return nullptr;
		}

		template<typename T>
		static VertexBuffer* CreateVertex(T* data, uint64_t elements) {

			if (GraphicsAPI::Get() == GraphicsAPI::OPEN_GL)
				return new OpenGLBuffer<T, BufferType::VERTEX>(data, elements);
			else HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");

			return nullptr;
		}

		template<typename T>
		static IndexBuffer* CreateIndex(T* data, uint64_t elements) {
			IndexBuffer* result = nullptr;
			if (GraphicsAPI::Get() == GraphicsAPI::OPEN_GL)
				result = new OpenGLBuffer<T, BufferType::INDEX>(data, elements);
			else HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
			if (result) {
				BufferLayout layout = { { ShaderDataType::Int, "index" } };
				result->SetLayout(layout);
			}
			return result;
		}
	};


}
