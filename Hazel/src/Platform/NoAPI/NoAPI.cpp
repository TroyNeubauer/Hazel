#include "hzpch.h"
#ifdef HZ_ENABLE_GRAPHICS_API_NONE

#include "NoAPI.h"
#include "Hazel/Core/Core.h"

struct GLFWwindow;

namespace Hazel {

	size_t NoAPIVertexArray::Bytes() const
	{
		return 0;
	}

	void NoAPIVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
		m_IndexBuffer = indexBuffer;
	}
	
	void NoAPIVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
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
		if (System::KBHit()) {
			if(m_Windows.size())
				HZ_CORE_INFO("Keypress detected. Closing windows");
			for (Window* window : m_Windows) {

				WindowCloseEvent* event = new WindowCloseEvent();
				(window->GetEventCallback())(event);
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
