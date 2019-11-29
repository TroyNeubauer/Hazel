#pragma once

#include <glm/vec2.hpp>
#include "Hazel/Core/Core.h"

namespace Hazel {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsKeyFired(int keycode);
		static bool IsMouseButtonPressed(int button);
		static inline float GetScrollDelta() { return s_ScrollDelta.y; }
		static inline glm::vec2 GetScrollDeltaVec() { return s_ScrollDelta; }

		static inline glm::vec2 GetMousePosition() { return s_MousePos; }
		static inline glm::vec2 GetMouseDelta() { return s_MouseDelta; }

		static void NextFrame();
	private:
		static glm::vec2 s_MousePos, s_LastMousePos, s_MouseDelta, s_ScrollDelta;

		friend class GLFWWindow;
	};

}
