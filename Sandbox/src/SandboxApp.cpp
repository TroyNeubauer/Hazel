#include <Hazel.h>
#include "Hazel/Util/Utils.h"
#include "Hazel/Layer/DebugLayer.h"

#include "imgui/imgui.h"

#include <stdio.h>

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

	}

	virtual void OnEvent(Hazel::Event* event) override
	{
	}

};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::DebugLayer());
	}

	~Sandbox()
	{

	}

};

Hazel::Application* Hazel::CreateApplication(int argc, char** argv)
{
	return new Sandbox();
}
