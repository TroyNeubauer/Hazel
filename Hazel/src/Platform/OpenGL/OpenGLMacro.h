#pragma once

#include "Hazel/Log.h"
#include <glad/glad.h>
#include <string>

inline static void GLClearError() { while (glGetError() != GL_NO_ERROR); }

static const std::string GLErrorToString(GLenum error)
{
	switch (error)
	{
	case GL_INVALID_INDEX:					return "INVALID INDEX";
	case GL_INVALID_ENUM:					return "INVALID ENUM";
	case GL_INVALID_VALUE:					return "INVALID VALUE";
	case GL_INVALID_OPERATION:				return "INVALID OPERATION";
	case GL_OUT_OF_MEMORY:					return "OUT OF MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION:	return "INVALID FRAMEBUFFER OPERATION";
	default: return std::string("Error code: ") + std::to_string(error);
	}
}

static bool GLLogCall(const char* function, const char* file, const int line)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)//Print all the errors
	{
		HZ_CORE_ERROR("[OpenGL] Error: {0}", GLErrorToString(error));
		HZ_CORE_ERROR("[OpenGL]     in function {0}", function);
		HZ_CORE_ERROR("[OpenGL]     at {0} ({1})", file, line);
		return false;
	}
	return true;
}

#if defined HZ_DEBUG
#define GLCall(x) GLClearError();\
                      x;\
                      HZ_CORE_ASSERT(GLLogCall(#x, __FILE__, __LINE__), "OpenGL Error!")
#else
#define GLCall(x) x
#endif
