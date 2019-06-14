#pragma once

#include "Hazel/Context/ContextManager.h"

namespace Hazel {
	class WindowsContextManager : public ContextManager
	{
	public:
		WindowsContextManager();
		virtual GraphicsContext* GetContext() override;
		virtual ~WindowsContextManager();

	private:
		GraphicsContext* m_Context = nullptr;
	};
}
