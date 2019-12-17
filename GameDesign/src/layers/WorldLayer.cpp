#include "WorldLayer.h"

#include <imgui.h>
#include <random>


WorldLayer::WorldLayer(Hazel::Ref<EditorShip>& originShip) : Hazel::Layer("World Layer"), m_Camera(new WorldCameraController())
{
	m_World.reset(new World(m_Camera));
	m_Ship = m_World->AddShip(originShip, glm::vec2(0.0f, 0.0f));
}

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
	if (m_Ship == nullptr)
	{
		if (!m_World->GetShips().empty())
			m_Ship = m_World->GetShips()[0];
	}
	else
	{
		m_Camera.SetPosition(m_Ship->GetCenterOfMass());
		m_Camera.SetZoom(m_Camera.GetZoom() * pow(1.2, Hazel::Input::GetScrollDelta()));
	}

	m_Camera.ForceUpdate();
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

}


void WorldLayer::Render()
{
	m_World->Render();
}

WorldLayer::~WorldLayer()
{

}

void WorldCameraController::Update(Hazel::Timestep ts, Hazel::Camera2D& camera)
{
	HZ_PROFILE_FUNCTION();


	camera.m_ZoomVel -= Hazel::Input::GetScrollDelta() * 3.0f;

	camera.m_Zoom += camera.m_ZoomVel * ts.Seconds();
	camera.m_ZoomVel *= (1.0f - ts.Seconds() * 5.0f);

	if (camera.m_Zoom <= 0.00001f)
		camera.m_Zoom = 0.00001f;
}
