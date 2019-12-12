
#include "GameDesign.h"
#include "layers/SandboxLayer.h"
#include "layers/WorldLayer.h"
#include "ship/Parts.h"

#include "Hazel.h"

GameDesign::GameDesign()
{
	Parts::Init();
	PushOverlay(new Hazel::DebugLayer());
	PushLayer(new SandboxLayer(new World()));
}

void GameDesign::Update()
{

}

void GameDesign::Render()
{
	Hazel::RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.6f), 1.0f));
	Hazel::RenderCommand::Clear();
	if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_5) || Hazel::Input::IsKeyPressed(HZ_KEY_H))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

GameDesign::~GameDesign()
{

}
