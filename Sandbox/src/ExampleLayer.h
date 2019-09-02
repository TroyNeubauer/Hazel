#pragma once

#include <Hazel.h>
#include "Sandbox.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	virtual void OnUpdate() override
	{

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Camera Info");
		
		//ImGui::DragFloat3("Light Position", &lightPosition.x, 0.5f);
		//ImGui::DragFloat3("Light Color", &lightColor.x, 0.01f, 0.0f, 1.0f);
		ImGui::Text("Altitude: %.1f", sandbox->m_Camera->GetStorage().GetPosition().y);
		ImGui::Text("X: %.1f, Z: %.1f", sandbox->m_Camera->GetStorage().GetPosition().x, sandbox->m_Camera->GetStorage().GetPosition().z);
		ImGui::Text("Pitch: %.1f, Yaw: %.1f, Roll: %.1f", degrees(sandbox->m_Camera->GetStorage().GetEulerAngles().x), 
			degrees(sandbox->m_Camera->GetStorage().GetEulerAngles().y), degrees(sandbox->m_Camera->GetStorage().GetEulerAngles().z));
		ImGui::End();
	}

	virtual void OnEvent(Hazel::Event* event) override
	{
		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::KeyPressed));
		dispatcher.Dispatch<Hazel::KeyReleasedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::KeyReleased));
	}

	bool KeyReleased(Hazel::KeyReleasedEvent* event)
	{
		return false;
	}

	bool KeyPressed(Hazel::KeyPressedEvent* event)
	{
		int code = event->GetKeyCode();

		return false;
	}

};