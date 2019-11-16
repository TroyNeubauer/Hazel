
#include "GameDesign.h"

#include "Hazel/glm.h"

GameDesign::GameDesign()
{
	PushOverlay(new Hazel::DebugLayer());
}

void GameDesign::Update()
{

}

void GameDesign::Render()
{
	Hazel::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 0.5f, 1.0f));
	Hazel::RenderCommand::Clear();
}

GameDesign::~GameDesign()
{

}