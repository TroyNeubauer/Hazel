#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	class Engine
	{
	public:
		static float GetDeltaTime();
		static float GetTime();
		static void SetDeltaTime(float delta);

		static void Update();

	private:
		static float m_DeltaTime;
		static float m_LastTime;
	};

	void Init();
	void Shutdown();
}
