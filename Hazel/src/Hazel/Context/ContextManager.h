#pragma once

#include "GraphicsContext.h"

namespace Hazel {

	class ContextManager
	{
	public:
		//Gets the already esisting ContextManager for this platform, or creates a new singleton if it doesn't yet exist
		static ContextManager* Get();
		//Releases any resources used by the manager including, but not limited to, the current context
		static inline void Destroy() { delete s_Instance; }
		//Creates a graphics context for the selected API if one hasn't been created already, and returns it
		virtual GraphicsContext* GetContext() = 0;

		virtual ~ContextManager() {}

	private:
		static ContextManager* s_Instance;
	};

}
