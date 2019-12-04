
#include "GameDesign.h"
#include "layers/SandboxLayer.h"
#include "layers/WorldLayer.h"

#include "Hazel.h"

GameDesign::GameDesign()
{
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
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
	}
}

GameDesign::~GameDesign()
{

}
