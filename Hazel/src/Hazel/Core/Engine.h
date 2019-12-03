#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	namespace Engine
	{
		bool IsStaticInitializationComplete();
		bool IsInitialized();

		float GetDeltaTime();
		float GetTime();
		void SetDeltaTime(float delta);

		void Update();

	};

	void Init();
	void Shutdown();
}
