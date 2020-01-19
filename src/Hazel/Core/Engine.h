#pragma once

#include "Hazel/Core/Core.h"
#include "Timestep.h"

namespace Hazel {

	namespace Engine
	{
		bool IsStaticInitializationComplete();
		bool IsInitialized();

		Timestep GetDeltaTime();
		float GetTime();
		void SetDeltaTime(float delta);

		void Update();

	};

	void Init();
	void Shutdown();
}
