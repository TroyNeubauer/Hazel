
#include "GameDesign.h"
#include "layers/SandboxLayer.h"
#include "layers/WorldLayer.h"
#include "ship/Parts.h"
#include "ship/Ships.h"

#include "Hazel.h"

#include <LabSound/LabSound.h>
#include <LabSound/extended/LabSound.h>
#include <thread>

Hazel::Ref<lab::AudioBus> sound;
Hazel::Ref<lab::SampledAudioNode> musicClipNode;
Hazel::Ref<lab::GainNode> gain;

GameDesign::GameDesign()
{
	Parts::Init();
	Ships::Init();
	PushOverlay(new Hazel::DebugLayer());
	PushLayer(new SandboxLayer(new World()));
	sound = lab::MakeBusFromFile("assets/audio/computers_have_control.mp3", true);
	gain = std::make_shared<lab::GainNode>();

	musicClipNode = std::make_shared<lab::SampledAudioNode>();
	{
		lab::ContextRenderLock r(&Application::GetAudioContext(), "Simple");
		musicClipNode->setBus(r, sound);
		musicClipNode->start(0.0);
	}
	GetAudioContext().connect(GetAudioContext().destination(), musicClipNode);

}

void GameDesign::Update(Hazel::Timestep ts)
{
	if (Hazel::Input::IsKeyFired(HZ_KEY_P))
	{
		HZ_INFO("Starting");
		musicClipNode->start(0.0);
	}
	if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
	{
		HZ_INFO("Is finished {}", musicClipNode->hasFinished());
	}
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
