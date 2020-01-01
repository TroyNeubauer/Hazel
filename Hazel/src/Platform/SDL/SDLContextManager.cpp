#include "hzpch.h"
#ifdef HZ_USE_SDL_CONTEXT_MANAGER
#include "SDLContextManager.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/OpenGLES/OpenGLESContext.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/NoAPI/NoAPI.h"

#include "SDL.h"

namespace Hazel {

	SDLContextManager::SDLContextManager()
	{
		if (GraphicsAPI::Get() != GraphicsAPIType::NONE) {
			HZ_PROFILE_SCOPE("SDL_Init()");
			int success = SDL_Init(SDL_INIT_VIDEO);
			HZ_CORE_ASSERT(success >= 0, "Could not intialize SDL!");
		}
	}

	GraphicsContext* SDLContextManager::GetContext()
	{
		if (!m_Context) {
			switch (GraphicsAPI::Get())
			{
#ifdef HZ_ENABLE_GRAPHICS_API_NONE
				case GraphicsAPIType::NONE:	return m_Context = new NoAPIContext();
#endif
#ifdef HZ_ENABLE_OPEN_GL
				case GraphicsAPIType::OPEN_GL:
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

					SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
					SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
					m_Context = new OpenGLContext();
					break;
#endif
#ifdef HZ_ENABLE_OPEN_GLES
				case GraphicsAPIType::OPEN_GLES:
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
					SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

					SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
					SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
					SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

					m_Context = new OpenGLESContext();
					break;
#endif
#ifdef HZ_ENABLE_VULKAN
				case GraphicsAPIType::VULKAN:
					
					
					m_Context = new VulkanContext();
					break;
#endif
				default: HZ_CORE_CRITICAL("Unsupported Graphics API {}", GraphicsAPI::ToString(GraphicsAPI::Get()));
			}	
		}
		return m_Context;
	}

	SDLContextManager::~SDLContextManager()
	{
		if (m_Context) {
			delete m_Context;
			if (GraphicsAPI::Get() != GraphicsAPIType::NONE)
			{
				HZ_PROFILE_CALL(SDL_Quit());
			}
		}
	}
}

#endif
