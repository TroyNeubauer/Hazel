#include "WorldLayer.h"

#include <imgui.h>

void WorldLayer::OnAttach()
{

}

void WorldLayer::OnDetach()
{

}

void WorldLayer::OnUpdate()
{
	m_World->Update();
}

void WorldLayer::OnEvent(Hazel::Event* event)
{
	
}

void WorldLayer::OnImGuiRender()
{
	ImGui::SliderFloat("G", &World::Constants::G, -0.01f, 10.0f);
}

void WorldLayer::Render()
{
	m_World->Render();
}

WorldLayer::~WorldLayer()
{

}
