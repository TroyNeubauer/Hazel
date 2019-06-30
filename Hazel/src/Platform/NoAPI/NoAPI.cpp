#include "hzpch.h"
#ifdef HZ_ENABLE_GRAPHICS_API_NONE

#include "NoAPI.h"

#ifdef HZ_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
	#include <conio.h>
#elif defined(HZ_PLATFORM_UNIX)
	#include "Platform/Unix/UnixWindow.h"
	#include <sys/ioctl.h>
	#include <termios.h>

	static bool _kbhit()
	{
		termios term;
		tcgetattr(0, &term);

		termios term2 = term;
		term2.c_lflag &= ~ICANON;
		tcsetattr(0, TCSANOW, &term2);

		int byteswaiting;
		ioctl(0, FIONREAD, &byteswaiting);

		tcsetattr(0, TCSANOW, &term);

		return byteswaiting > 0;
	}
#else
	#error
#endif

struct GLFWwindow;

namespace Hazel {


	size_t NoAPIVertexArray::Bytes() const
	{
		return 0;
	}

	void NoAPIVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
	
	void NoAPIVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		m_VertexBuffers.push_back(vertexBuffer);
	}

	const std::vector<std::shared_ptr<VertexBuffer>>& NoAPIVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}
	
	const std::shared_ptr<IndexBuffer>& NoAPIVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	NoAPIContext::NoAPIContext()
	{
		PreInit();
		HZ_CORE_TRACE("Creating NoAPI Context");
		HZ_CORE_INFO("Press any key to exit");
	}

	void NoAPIContext::SwapBuffers() {
		if (_kbhit()) {
			if(m_Windows.size())
				HZ_CORE_INFO("Keypress detected. Closing windows");
			for (Window* window : m_Windows) {
#ifdef HZ_PLATFORM_WINDOWS
				WindowsWindow* myWindow = (WindowsWindow*) window;
#elif defined(HZ_PLATFORM_UNIX)
				UnixWindow* myWindow = (UnixWindow*) window;
#endif
				WindowCloseEvent* event = new WindowCloseEvent();
				(myWindow->GetEventCallback())(event);
			}
			m_Windows.clear();
		}

	}

	void NoAPIContext::AddWindow(Window* window)
	{
		m_Windows.push_back(window);
	}
	
	void NoAPIContext::RemoveWindow(Window* window) 
	{
		auto it = std::find(m_Windows.begin(), m_Windows.end(), window);
		if (it != m_Windows.end())
			m_Windows.erase(it);
	}

	GraphicsAPIType NoAPIContext::GetAPIType()
	{
		return GraphicsAPIType::NONE;
	}
	
	ImGuiLayer* NoAPIContext::CreateImGuiLayer()
	{
		return new NoAPIImGuiLayer();
	}

}


#endif