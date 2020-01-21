#include "Sandbox.h"
#include "ExampleLayer.h"

#include <Hazel/Core/EntryPoint.h>


Sandbox::Sandbox()
{

	PushLayer(new ExampleLayer());
	PushOverlay(new Hazel::DebugLayer());
	HZ_INFO("Created app");

}

void Sandbox::Update(Hazel::Timestep ts)
{

}

void Sandbox::Render()
{

	Hazel::RenderCommand::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	Hazel::RenderCommand::Clear();

}

Sandbox::~Sandbox()
{

}

Hazel::Application* HZCreateApplication(int argc, char** argv)
{
	return new Sandbox();
}

void HZGetApplicationInfo(Hazel::ApplicationInfo* info)
{
	info->Name = "Sandbox";
}

