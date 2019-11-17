#include "WorldLayer.h"

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

}

void WorldLayer::Render()
{
	m_World->Render();
}

WorldLayer::~WorldLayer()
{

}
