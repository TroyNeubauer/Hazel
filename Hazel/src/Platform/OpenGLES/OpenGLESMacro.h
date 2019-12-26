#pragma once
#ifdef HZ_ENABLE_OPEN_GLES
#include "OpenGLES.h"
#include "Hazel/Core/Log.h"

#include <stdio.h>

#define GLCall(x) GLClearError(); x; HZ_CORE_ASSERT(GLLogCall(__FUNCTION__, __FILE__, __LINE__), "OpenGL Error!")

inline static void GLClearError() { while (glGetError() != GL_NO_ERROR); }

static inline const char* GLErrorToString(GLenum error)
{

	static char temp[256];
	switch (error)
	{
		case GL_INVALID_INDEX:					return "INVALID INDEX";
		case GL_INVALID_ENUM:					return "INVALID ENUM";
		case GL_INVALID_VALUE:					return "INVALID VALUE";
		case GL_INVALID_OPERATION:				return "INVALID OPERATION";
		case GL_OUT_OF_MEMORY:					return "OUT OF MEMORY";
		case GL_INVALID_FRAMEBUFFER_OPERATION:	return "INVALID FRAMEBUFFER OPERATION";
		default: snprintf(temp, sizeof(temp), "Unknown GL error code: %d", error); return temp;
	}
}

static inline bool GLLogCall(const char* function, const char* file, const int line)
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

#endif