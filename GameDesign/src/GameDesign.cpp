
#include "GameDesign.h"
#include "layers/WorldLayer.h"

#include "Hazel.h"

GameDesign::GameDesign()
{
	PushOverlay(new Hazel::DebugLayer());
	PushLayer(new WorldLayer(new World()));
	Hazel::Renderer2D::Init();
}

void GameDesign::Update()
{

}

void GameDesign::Render()
{
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
	Hazel::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 0.5f, 1.0f));
	Hazel::RenderCommand::Clear();
}

GameDesign::~GameDesign()
{
	Hazel::Renderer2D::Shutdown();
}