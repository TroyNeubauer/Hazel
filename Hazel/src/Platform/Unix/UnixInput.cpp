#include "hzpch.h"
#ifdef HZ_PLATFORM_UNIX

#include "UnixInput.h"

#include "Hazel/Application.h"
#include <GLFW/glfw3.h>

namespace Hazel {

	Input* Input::s_Instance = new UnixInput();

	bool UnixInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool UnixInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> UnixInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float UnixInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return x;
	}

	float UnixInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return y;
	}

}

#endif