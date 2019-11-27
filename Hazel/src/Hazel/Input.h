#pragma once

#include "Hazel/Core.h"
#include <glm/vec2.hpp>

namespace Hazel {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsKeyFired(int keycode);
		static bool IsMouseButtonPressed(int button);

		static inline glm::vec2 GetMousePosition() { return s_MousePos; }
		static inline glm::vec2 GetMouseDelta() { return s_MouseDelta; }

		static void NextFrame();
	private:
		static glm::vec2 s_MousePos, s_LastMousePos, s_MouseDelta;
	};

}