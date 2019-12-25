#include "hzpch.h"
#ifdef HZ_SDL_INPUT

#include "Hazel/Core/Input.h"
#include "Hazel/Core/Application.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Context/ContextManager.h"

#include "SDL.h"

#include <bitset>

namespace Hazel {

	glm::vec2 Input::s_MousePos(-1.0f, -1.0f), Input::s_LastMousePos(-1.0f, -1.0f),
			Input::s_MouseDelta(0.0f, 0.0f), Input::s_ScrollDelta(0.0f, 0.0f);

	static std::bitset<HZ_MAX_KEY_VALUE> s_LastKeys, s_CurrentKeys;
	static std::bitset<32> s_MouseButtons;

	bool Input::IsKeyPressed(int keycode)
	{
		return s_CurrentKeys[keycode];
	}

	bool Input::IsKeyFired(int keycode)
	{
		if (keycode < HZ_MIN_KEY_VALUE || keycode >= HZ_MAX_KEY_VALUE)
			return false;
		return s_CurrentKeys[keycode] && !s_LastKeys[keycode];
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		return s_MouseButtons[button];
	}

	void Input::NextFrame()
	{
		HZ_PROFILE_FUNCTION();

		s_ScrollDelta = { 0.0f, 0.0f };
		s_LastMousePos = s_MousePos;

		static std::bitset<HZ_MAX_KEY_VALUE> newKeys;
		newKeys.reset();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			Event* hzEvent = nullptr;

			int buttonID;
			switch (event.type) {
				case SDL_MOUSEMOTION:

					s_MousePos.x = event.motion.x;
					s_MousePos.y = event.motion.y;
					s_MouseDelta = s_MousePos - s_LastMousePos;
					hzEvent = new MouseMovedEvent(event.motion.x, event.motion.y);
					break;
					
				case SDL_MOUSEBUTTONUP:
					if (event.button.which == SDL_TOUCH_MOUSEID) buttonID = 0;
					else buttonID = event.button.which;

					s_MouseButtons[buttonID] = false;

					hzEvent = new MouseButtonReleasedEvent(buttonID);
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (event.button.which == SDL_TOUCH_MOUSEID) buttonID = 0;
					else buttonID = event.button.which;

					s_MouseButtons[buttonID] = true;

					hzEvent = new MouseButtonPressedEvent(buttonID);
					break;

				case SDL_MOUSEWHEEL:
					s_ScrollDelta = { event.wheel.x, event.wheel.y };

					hzEvent = new MouseScrolledEvent(event.wheel.x, event.wheel.y);
					break;

				case SDL_KEYDOWN:
					newKeys[event.key.keysym.sym] = true;
					hzEvent = new KeyPressedEvent(event.key.keysym.sym, event.key.repeat);
					break;

				case SDL_KEYUP:
					newKeys[event.key.keysym.sym] = false;
					hzEvent = new KeyReleasedEvent(event.key.keysym.sym);
					break;

				case SDL_TEXTEDITING:
					break;

				case SDL_TEXTINPUT:
					break;

				case SDL_WINDOWEVENT_RESIZED:
					hzEvent = new WindowResizeEvent(event.window.data1, event.window.data2);
					for (Window* window : ContextManager::Get()->GetContext()->GetWindows())
					{
						if (SDL_GetWindowID(reinterpret_cast<SDL_Window*>(window->GetNativeWindow())) == event.window.windowID)
						{
							ContextManager::Get()->GetContext()->OnWindowResize(window, event.window.data1, event.window.data2);
						}
					}
					break;

				case SDL_WINDOWEVENT_CLOSE:
					hzEvent = new WindowCloseEvent();
					break;


			}
			if (hzEvent) Application::Get().GetWindow().GetEventCallback()(hzEvent);
		}

		for (int i = HZ_MIN_KEY_VALUE; i < HZ_MAX_KEY_VALUE; i++)
		{
			s_LastKeys[i] = s_CurrentKeys[i];

			s_CurrentKeys[i] = newKeys[i];
			
		}

	}

}

#endif
