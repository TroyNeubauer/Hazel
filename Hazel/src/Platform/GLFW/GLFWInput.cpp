#include "hzpch.h"
#ifdef HZ_GLFW_INPUT

#include "Hazel/Core/Input.h"
#include "Hazel/Core/Application.h"
#include "Hazel/Core/KeyCodes.h"

#include "GLFW.h"

#include <bitset>

namespace Hazel {

	glm::vec2 Input::s_MousePos(-1.0f, -1.0f), Input::s_LastMousePos(-1.0f, -1.0f),
			Input::s_MouseDelta(0.0f, 0.0f), Input::s_ScrollDelta(0.0f, 0.0f);

	static std::bitset<HZ_MAX_KEY_VALUE> s_LastKeys, s_CurrentKeys;

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsKeyFired(int keycode)
	{
		if (keycode < HZ_MIN_KEY_VALUE || keycode >= HZ_MAX_KEY_VALUE)
			return false;
		return s_CurrentKeys[keycode] && !s_LastKeys[keycode];
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	static glm::vec2 GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	bool Input::DidMouseMove()
	{
		return s_MouseDelta.x || s_MouseDelta.y;
	}

	void Input::NextFrame()
	{
		HZ_PROFILE_FUNCTION();

		s_ScrollDelta = { 0.0f, 0.0f };
		s_LastMousePos = s_MousePos;
		glm::vec2 newMousePos = GetMousePositionImpl();
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		for (int i = HZ_MIN_KEY_VALUE; i < HZ_MAX_KEY_VALUE; i++)
		{
			s_LastKeys[i] = s_CurrentKeys[i];

			bool presed = glfwGetKey(window, i) == GLFW_PRESS;
			s_CurrentKeys[i] = presed;
			
		}

		if(s_MousePos != glm::vec2(-1.0f, -1.0f))
			s_MouseDelta = newMousePos - s_LastMousePos;
		s_MousePos = newMousePos;

	}

}

#endif
