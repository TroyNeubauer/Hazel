#include "hzpch.h"
#ifdef HZ_ENABLE_OPEN_GLES

#include "Hazel/ImGui/ImGuiLayer.h"
#include "OpenGLESImGuiLayer.h"

#include "Hazel/Core/Application.h"
#include "Platform/SDL/SDL.h"
#include "Platform/SDL/SDLWindow.h"

#include "OpenGLES.h"
#include "OpenGLESMacro.h"

#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl3.h>


namespace Hazel {

	void OpenGLESImGuiLayer::OnAttach()
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
		//No viewports on GLES
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		{
			HZ_PROFILE_SCOPE("ImGui::StyleColorsDark()");
			ImGui::StyleColorsDark();
		}

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		SDL_Window* window = reinterpret_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());
		{
			HZ_PROFILE_SCOPE("ImGui_ImplGlfw_InitForOpenGL()");
			ImGui_ImplSDL2_InitForOpenGL(window, reinterpret_cast<Hazel::SDLWindow*>(&Application::Get().GetWindow())->m_Context);
		}
		{
			HZ_PROFILE_SCOPE("ImGui_ImplOpenGL3_Init(\"#version 100\")");
			ImGui_ImplOpenGL3_Init("#version 100");
		}
	}

	void OpenGLESImGuiLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void OpenGLESImGuiLayer::OnEvent(Event* event) {
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
		else if (flags & (EventCategory::EventCategoryKeyboard))
		{
			if (io.WantCaptureKeyboard)
				event->Handled = true;
		}
	}

	void OpenGLESImGuiLayer::Begin()
	{
		HZ_PROFILE_FUNCTION();
		{
			HZ_PROFILE_SCOPE("ImGui_ImplOpenGL3_NewFrame");
			GLCall(ImGui_ImplOpenGL3_NewFrame());
		}
		{
			HZ_PROFILE_SCOPE("ImGui_ImplSDL2_NewFrame");
			ImGui_ImplSDL2_NewFrame(reinterpret_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow()));
		}
		{
			HZ_PROFILE_SCOPE("ImGui::NewFrame");
			ImGui::NewFrame();
		}
	}

	void OpenGLESImGuiLayer::End()
	{
		HZ_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float) app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		{
			HZ_PROFILE_SCOPE("ImGui::Render");
			ImGui::Render();
		}
		{
			HZ_PROFILE_SCOPE("ImGui_ImplOpenGL3_RenderDrawData");
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void OpenGLESImGuiLayer::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();
	}

}

#endif
