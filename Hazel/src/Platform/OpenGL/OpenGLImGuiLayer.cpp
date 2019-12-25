#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GL

#include "Hazel/ImGui/ImGuiLayer.h"
#include "OpenGLImGuiLayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Hazel/Core/Application.h"

#include "Platform/GLFW3/GLFW3.h"
#include "OpenGL.h"


namespace Hazel {

	void OpenGLImGuiLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		{
			HZ_PROFILE_SCOPE("ImGui::CreateContext()");
			ImGui::CreateContext();
		}
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		{
			HZ_PROFILE_SCOPE("ImGui::StyleColorsDark()");
			ImGui::StyleColorsDark();
		}

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		{
			HZ_PROFILE_SCOPE("ImGui_ImplGlfw_InitForOpenGL()");
			ImGui_ImplGlfw_InitForOpenGL(window, false);
		}
		{
			HZ_PROFILE_SCOPE("ImGui_ImplOpenGL3_Init(\"#version 410\")");
			ImGui_ImplOpenGL3_Init("#version 410");
		}
	}

	void OpenGLImGuiLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void OpenGLImGuiLayer::OnEvent(Event* event) {
		ImGuiIO& io = ImGui::GetIO();
		int flags = event->GetCategoryFlags();
		if (flags & EventCategory::EventCategoryKeyboard)
		{
			if (io.WantCaptureKeyboard)
				event->Handled = true;
		}
		if (flags & (EventCategory::EventCategoryMouse | EventCategory::EventCategoryMouseButton))
		{
			if (io.WantCaptureMouse)
				event->Handled = true;
		}
	}

	void OpenGLImGuiLayer::Begin()
	{
		HZ_PROFILE_FUNCTION();
		{
			HZ_PROFILE_SCOPE("ImGui_ImplOpenGL3_NewFrame");
			ImGui_ImplOpenGL3_NewFrame();
		}
		{
			HZ_PROFILE_SCOPE("ImGui_ImplGlfw_NewFrame");
			ImGui_ImplGlfw_NewFrame();
		}
		{
			HZ_PROFILE_SCOPE("ImGui::NewFrame");
			ImGui::NewFrame();
		}
	}

	void OpenGLImGuiLayer::End()
	{
		HZ_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		{
			HZ_PROFILE_SCOPE("");
			ImGui::Render();
		}
		{
			HZ_PROFILE_SCOPE("");
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			HZ_PROFILE_SCOPE("Viewports Update");
			GLFWwindow* backupContext = glfwGetCurrentContext();
			{
				HZ_PROFILE_SCOPE("ImGui::UpdatePlatformWindows");
				ImGui::UpdatePlatformWindows();
			}
			{
				HZ_PROFILE_SCOPE("ImGui::RenderPlatformWindowsDefault()");
				ImGui::RenderPlatformWindowsDefault();
			}
			{
				HZ_PROFILE_SCOPE("glfwMakeContextCurrent");
				glfwMakeContextCurrent(backupContext);
			}
		}
	}

	void OpenGLImGuiLayer::OnImGuiRender()
	{
		
	}

}

#endif
