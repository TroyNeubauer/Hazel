#pragma once

#include "Hazel/Core.h"

namespace Hazel {

	class Engine
	{
	public:
		static float GetDeltaTime();
		static float GetTime();
		static void SetDeltaTime(float delta);

	private:
		static float m_DeltaTime;
	};
}
