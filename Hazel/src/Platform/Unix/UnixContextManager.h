#pragma once
#include "../../Hazel/Core/Core.h"
#ifdef HZ_PLATFORM_UNIX

#include "Hazel/Context/ContextManager.h"

namespace Hazel {
	class UnixContextManager : public ContextManager
	{
	public:
		UnixContextManager();
		virtual GraphicsContext* GetContext() override;

		virtual ~UnixContextManager();

	private:
		GraphicsContext* m_Context = nullptr;
	};
}

#endif
