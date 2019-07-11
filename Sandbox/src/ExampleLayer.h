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
		if (launching && GetTime() > 100.0f) {
			launching = false;
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Light Settings");
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
		if (event->GetKeyCode() == HZ_KEY_ESCAPE)
			paused = !paused;
		return false;
	}

	bool KeyPressed(Hazel::KeyPressedEvent* event)
	{
		int code = event->GetKeyCode();
		if (code == HZ_KEY_L)//Launch
		{
			launching = true;
			launchTime = Hazel::Engine::GetTime();
		}
		else if (code == HZ_KEY_C)//Cancel
		{
			if (launching)
			{
				launching = false;
				if (sandbox->m_Camera->GetStorage().GetPosition().y > 30.0f)
				{
					vec3 pos = sandbox->m_Camera->GetStorage().GetPosition();
					pos.y = 10.0f;
					sandbox->m_Camera->GetStorage().SetPosition(pos);
					sandbox->m_Camera->ForceUpdate();
				}
			}
		}
		else if (code == HZ_KEY_G)
		{
			generating = !generating;
		}
		else if (code == HZ_KEY_J)
		{
			sandbox->m_Camera->GetStorage().SetPosition(vec3(0.0f, 6900.0f, 0.0f));
			sandbox->m_Camera->ForceUpdate();
		}
		else if (code == HZ_KEY_R)
		{
			sandbox->m_Camera->GetStorage().SetEulerAngles(vec3(0.0f, 0.0f, 0.0f));
			sandbox->m_Camera->ForceUpdate();
		}
		return false;
	}

};