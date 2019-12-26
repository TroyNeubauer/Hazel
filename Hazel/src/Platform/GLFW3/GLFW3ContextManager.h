#pragma once
#include "Hazel/Core/Core.h"
#ifdef HZ_USE_GLFW3_CONTEXT_MANAGER

#include "Hazel/Context/ContextManager.h"

namespace Hazel {
	class GLFW3ContextManager : public ContextManager
	{
	public:
		GLFW3ContextManager();
		virtual GraphicsContext* GetContext() override;

		virtual ~GLFW3ContextManager();

	private:
		GraphicsContext* m_Context = nullptr;
	};
}

#endif
