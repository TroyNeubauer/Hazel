#pragma once
#include "Hazel/Core/Core.h"
#ifdef HZ_USE_SDL_CONTEXT_MANAGER

#include "Hazel/Context/ContextManager.h"

namespace Hazel {
	class SDLContextManager : public ContextManager
	{
	public:
		SDLContextManager();
		virtual GraphicsContext* GetContext() override;

		virtual ~SDLContextManager();

	private:
		GraphicsContext* m_Context = nullptr;
	};
}

#endif
