#include "hzpch.h"
#include "Buffer.h"

#include "Hazel/Core/glm.h"
#include "Platform/NoAPI/NoAPI.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel {
	uint32_t SizeOfShaderDataType(ShaderDataType type)
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

	template<> ShaderDataType ToShaderDataType<float>()			{ return ShaderDataType::Float32; }
	template<> ShaderDataType ToShaderDataType<double>()			{ return ShaderDataType::Float64; }
	template<> ShaderDataType ToShaderDataType<glm::vec2>()		{ return ShaderDataType::Float32_2; }
	template<> ShaderDataType ToShaderDataType<glm::dvec2>()		{ return ShaderDataType::Float64_2; }
	template<> ShaderDataType ToShaderDataType<glm::vec3>()		{ return ShaderDataType::Float32_3; }
	template<> ShaderDataType ToShaderDataType<glm::dvec3>()		{ return ShaderDataType::Float64_3; }
	template<> ShaderDataType ToShaderDataType<glm::vec4>()		{ return ShaderDataType::Float32_4; }
	template<> ShaderDataType ToShaderDataType<glm::dvec4>()		{ return ShaderDataType::Float64_4; }

	template<> ShaderDataType ToShaderDataType<uint8_t>()		{ return ShaderDataType::Int8; }
	template<> ShaderDataType ToShaderDataType<uint16_t>()		{ return ShaderDataType::Int16; }
	template<> ShaderDataType ToShaderDataType<uint32_t>()		{ return ShaderDataType::Int32; }
	template<> ShaderDataType ToShaderDataType<uint64_t>()		{ return ShaderDataType::Int64; }

	template<> ShaderDataType ToShaderDataType<glm::i8vec2>()	{ return ShaderDataType::Int8_2; }
	template<> ShaderDataType ToShaderDataType<glm::i16vec2>()	{ return ShaderDataType::Int16_2; }
	template<> ShaderDataType ToShaderDataType<glm::i32vec2>()	{ return ShaderDataType::Int32_2; }
	template<> ShaderDataType ToShaderDataType<glm::i64vec2>()	{ return ShaderDataType::Int64_2; }

	template<> ShaderDataType ToShaderDataType<glm::i8vec3>()	{ return ShaderDataType::Int8_3; }
	template<> ShaderDataType ToShaderDataType<glm::i16vec3>()	{ return ShaderDataType::Int16_3; }
	template<> ShaderDataType ToShaderDataType<glm::i32vec3>()	{ return ShaderDataType::Int32_3; }
	template<> ShaderDataType ToShaderDataType<glm::i64vec3>()	{ return ShaderDataType::Int64_3; }

	template<> ShaderDataType ToShaderDataType<glm::i8vec4>()	{ return ShaderDataType::Int8_4; }
	template<> ShaderDataType ToShaderDataType<glm::i16vec4>()	{ return ShaderDataType::Int16_4; }
	template<> ShaderDataType ToShaderDataType<glm::i32vec4>()	{ return ShaderDataType::Int32_4; }
	template<> ShaderDataType ToShaderDataType<glm::i64vec4>()	{ return ShaderDataType::Int64_4; }

	template<> ShaderDataType ToShaderDataType<glm::mat3>()		{ return ShaderDataType::Mat3; }
	template<> ShaderDataType ToShaderDataType<glm::mat4>()		{ return ShaderDataType::Mat4; }
	template<> ShaderDataType ToShaderDataType<bool>()			{ return ShaderDataType::Bool; }

	template<>
	Ref<VertexBuffer> VertexBuffer::Create(float* data, uint64_t bytes)
	{
		switch (GraphicsAPI::Get())
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case GraphicsAPIType::NONE:		return Ref<VertexBuffer>(new NoAPIBuffer <float, BufferType::VERTEX>(data, bytes));
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case GraphicsAPIType::OPEN_GL:	return Ref<VertexBuffer>(new OpenGLBuffer<float, BufferType::VERTEX>(data, bytes));
#endif
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}

	template<>
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint64_t bytes)
	{
		switch (GraphicsAPI::Get())
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case GraphicsAPIType::NONE:		return Ref<IndexBuffer>(new NoAPIBuffer <uint32_t, BufferType::INDEX>(data, bytes));
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case GraphicsAPIType::OPEN_GL:	return Ref<IndexBuffer>(new OpenGLBuffer<uint32_t, BufferType::INDEX>(data, bytes));
#endif
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}

	template<>
	Ref<VertexBuffer> VertexBuffer::CreateVertex(uint64_t bytes)
	{
		switch (GraphicsAPI::Get())
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case GraphicsAPIType::NONE:		return Ref<VertexBuffer>(new NoAPIBuffer <float, BufferType::VERTEX>(bytes));
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case GraphicsAPIType::OPEN_GL:	return Ref<VertexBuffer>(new OpenGLBuffer<float, BufferType::VERTEX>(bytes));
#endif
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}


	template<>
	Ref<IndexBuffer> IndexBuffer::CreateIndex(uint64_t bytes)
	{
		switch (GraphicsAPI::Get())
		{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
			case GraphicsAPIType::NONE:		return Ref<IndexBuffer>(new NoAPIBuffer <uint32_t, BufferType::INDEX>(bytes));
#endif
#ifdef HZ_ENABLE_OPEN_GL
			case GraphicsAPIType::OPEN_GL:	return Ref<IndexBuffer>(new OpenGLBuffer<uint32_t, BufferType::INDEX>(bytes));
#endif
			default: HZ_CORE_ASSERT(false, "Buffer cannot be created from graphics API");
		}
		return nullptr;
	}

}

