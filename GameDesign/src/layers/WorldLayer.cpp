#include "WorldLayer.h"

#include "ship/Part.h"
#include "ship/Parts.h"

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

	Ship* ship = m_Ship.get();
	
	m_Ship->ForEachPartIfType<EnginePart>([ship, ts](EnginePart& part) {
		float change = 0.0f;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) change -= ts.Seconds() * 5.0f;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) change += ts.Seconds() * 5.0f;

		if (change == 0.0f)
		{
			change = -part.GetGimbal() * 80.0f * ts.Seconds();
		}
		part.SetGimbal(part.GetGimbal() + change);
		part.SetThrottle(ship->GetThrottle());
	});
	
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
	ImGui::Begin("Ship");
	ImGui::Text("pos {%.2f, %.2f} m", m_Ship->GetCenterOfMass().x, m_Ship->GetCenterOfMass().y);
	ImGui::Text("rotation %.2f degrees", glm::degrees(m_Ship->GetRotation()));
	ImGui::Text("mass %.2f kg", m_Ship->GetPhsicsBody()->GetMass());
	ImGui::Text("speed %.3f m/s", m_Ship->GetPhsicsBody()->GetLinearVelocity().Length());
	ImGui::Text("speed %.2f degrees/s", glm::degrees(m_Ship->GetPhsicsBody()->GetAngularVelocity()));

	if (ImGui::Button("Break"))
	{
		int distance = -1;
		std::vector<Hazel::Ref<Part>> partsToStage;
		for (auto& part : m_Ship->GetParts())
		{
			int currentDistance = 0;
			Part* partPtr = part.get();
			while (!partPtr->IsRoot())
			{
				currentDistance++;
				partPtr->Advance(partPtr);
			}
			//Root part
			if (currentDistance == 0) continue;
			//Reset for new maxes
			if (distance == -1 || currentDistance > distance)
			{
				partsToStage.clear();
				distance = currentDistance;
			}

			//Add all parts that match the best distance found so far
			if (distance == currentDistance)
			{
				partsToStage.push_back(part);
			}
		}

		for (Hazel::Ref<Part> part : partsToStage)
		{
			Ship* newShip = m_Ship->Split(*m_World, part.get());
			if (newShip) m_World->AddShip(newShip);
		}
	}

	if (ImGui::Button("Stage"))
	{
		int distance = -1;
		std::vector<Hazel::Ref<Part>> partsToStage;
		for (auto& part : m_Ship->GetParts())
		{
			int currentDistance = 0;
			Part* partPtr = part.get();
			if (!dynamic_cast<DecouplerPart*>(partPtr)) continue;

			while (!partPtr->IsRoot())
			{
				currentDistance++;
				partPtr->Advance(partPtr);
			}
			//Root part
			if (currentDistance == 0) continue;
			//Reset for new maxes
			if (distance == -1 || currentDistance > distance)
			{
				partsToStage.clear();
				distance = currentDistance;
			}

			//Add all parts that match the best distance found so far
			if (distance == currentDistance)
			{
				partsToStage.push_back(part);
			}
		}

		for (Hazel::Ref<Part> part : partsToStage)
		{
			DecouplerPart* dec = dynamic_cast<DecouplerPart*>(part.get());
			Ship* newShip = dec->Release(*m_World, *m_Ship);
			if (newShip) m_World->AddShip(newShip);
		}
	}

	if (ImGui::SliderFloat("Throttle", &m_Ship->GetThrottle(), 0.0f, 1.0f))
	{
		Ship* ship = m_Ship.get();
		m_Ship->ForEachPartIfType<EnginePart>([ship](EnginePart& part) {
			part.SetThrottle(ship->GetThrottle());
		});
	}
	ImGui::End();
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


	camera.m_ZoomVel -= Hazel::Input::GetScrollDelta() * 2.0f;

	camera.m_Zoom += camera.m_ZoomVel * ts.Seconds();
	camera.m_ZoomVel *= (1.0f - ts.Seconds() * 5.0f);

	if (camera.m_Zoom <= 0.00001f)
		camera.m_Zoom = 0.00001f;


}
