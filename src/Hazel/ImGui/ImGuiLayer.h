#pragma once

#include "Hazel/Layer/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	//Represents the ImGui layer
	//There are subclasses for each render API
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") {}

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event* event);

		virtual void Begin() = 0;
		virtual void End() = 0;
	};

}