
#include "GameDesign.h"
#include "layers/SandboxLayer.h"
#include "layers/WorldLayer.h"
#include "layers/EditorLayer.h"

#include <Hazel.h>
#include <LabSound/LabSound.h>
#include <LabSound/extended/LabSound.h>
#include <thread>

Hazel::Ref<Hazel::AudioSource> sound;

GameDesign* GameDesign::s_Instance;

GameDesign::GameDesign()
{
	s_Instance = this;

	Parts::Init();
	Ships::Init();
	PushOverlay(new Hazel::DebugLayer());

	PushLayer(new EditorLayer());
	sound = Hazel::AudioManager::Load("assets/audio/computers_have_control.mp3");

}

void GameDesign::Update(Hazel::Timestep ts)
{
	if (Hazel::Input::IsKeyFired(HZ_KEY_P))
	{
		HZ_INFO("Starting");
		Hazel::AudioManager::Play(sound);
	}
}

void GameDesign::Render()
{
	Hazel::RenderCommand::SetClearColor(glm::vec4(glm::vec3(0.6f), 1.0f));
	Hazel::RenderCommand::Clear();

}

Hazel::Ref<EditorShip>& GameDesign::CreateShip()
{
	return m_EditorShips.emplace_back(new EditorShip());
}

GameDesign::~GameDesign()
{

}
