#include "hzpch.h"
#ifdef HZ_GLFW_INPUT

#include "Hazel/Input.h"
#include "Hazel/Application.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	glm::vec2 Input::s_MousePos(-1.0f, -1.0f), Input::s_LastMousePos(-1.0f, -1.0f), Input::s_MouseDelta(0.0f, 0.0f);

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
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

	void Input::NextFrame()
	{
		s_LastMousePos = s_MousePos;
		glm::vec2 newMousePos = GetMousePositionImpl();

		if(s_MousePos != glm::vec2(-1.0f, -1.0f))
			s_MouseDelta = newMousePos - s_LastMousePos;
		s_MousePos = newMousePos;
	}


}

#endif