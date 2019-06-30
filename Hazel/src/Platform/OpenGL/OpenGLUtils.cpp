#include "hzpch.h"
#include "OpenGLUtils.h"

namespace Hazel {
	GLenum OpenGLUtils::ShaderDataTypeToGLType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::None:		return 0;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Float16:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_2:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_3:	return GL_HALF_FLOAT;
		case ShaderDataType::Float16_4:	return GL_HALF_FLOAT;
		case ShaderDataType::Float64:	return GL_DOUBLE;
		case ShaderDataType::Float64_2:	return GL_DOUBLE;
		case ShaderDataType::Float64_3:	return GL_DOUBLE;
		case ShaderDataType::Float64_4:	return GL_DOUBLE;
		case ShaderDataType::Int64:		return GL_INT;
		case ShaderDataType::Int64_2:	return GL_INT;
		case ShaderDataType::Int64_3:	return GL_INT;
		case ShaderDataType::Int64_4:	return GL_INT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Int16:		return GL_SHORT;
		case ShaderDataType::Int16_2:	return GL_SHORT;
		case ShaderDataType::Int16_3:	return GL_SHORT;
		case ShaderDataType::Int16_4:	return GL_SHORT;
		case ShaderDataType::Int8:		return GL_BYTE;
		case ShaderDataType::Int8_2:	return GL_BYTE;
		case ShaderDataType::Int8_3:	return GL_BYTE;
		case ShaderDataType::Int8_4:	return GL_BYTE;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Bool:		return GL_BOOL;
		default:
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}
		return 0;
	}

}

