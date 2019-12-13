#include "WorldLayer.h"

#include <imgui.h>
#include <random>


void WorldLayer::OnAttach()
{
	std::default_random_engine gen;
	std::uniform_real_distribution<float> pos(-6.0f, 6.0f);
	for (int i = 0; i < 2; i++)
	{
		//Part& part = m_World->AddPart({ pos(gen), pos(gen) }, Parts::MK1Capsule);
		
	}
}

void WorldLayer::OnDetach()
{

}

void WorldLayer::OnUpdate(Hazel::Timestep ts)
{
	m_World->Update(ts);
}

void WorldLayer::OnEvent(Hazel::Event* event)
{
	
}

static bool SliderDouble(const char* label, double* v, double v_min, double v_max, const char* format, double power)
{
	return ImGui::SliderScalar(label, ImGuiDataType_Double, v, &v_min, &v_max, format, power);
}

void WorldLayer::OnImGuiRender()
{
	SliderDouble("G", &World::Constants::G, -0.01f, 10.0f, "%.5f", 1.0f);
}


void WorldLayer::Render()
{
	m_World->Render();
}

WorldLayer::~WorldLayer()
{

}
