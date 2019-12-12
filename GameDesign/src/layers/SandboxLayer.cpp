#include "SandboxLayer.h"

#include "ship/Part.h"
#include "ship/Parts.h"
#include "ship/Ship.h"
#include "ship/Ships.h"

#include <imgui.h>
#include <random>

void SandboxLayer::OnAttach()
{
    std::random_device rd;
    std::mt19937 mt(rd());
	std::uniform_real_distribution<float> pos(-15.0f, 15.0f);

	/*for (int i = 0; i < 2; i++)
	{
		m_World->AddShip(ship, { pos(mt), pos(mt) }, 0.0f);
	}*/
	for (int i = 0; i < 2; i++)
	{
		m_World->AddShip(Ships::MK1Ship, { pos(mt), pos(mt) }, 0.0f);
	}
	m_World->SetBodyRemovedCallback([this](Body* body) {
		if (body == m_SelectedBody) m_SelectedBody = nullptr;
		if (body == m_DraggedBody)
		{
			m_DraggedBody = nullptr;
			m_MouseDragged = false;
		}
		
	});
	m_World->GetCamera().SetZoom(10.0f);
}

void SandboxLayer::OnDetach()
{

}

float lastMoved = 0.0f;

void SandboxLayer::OnUpdate()
{
	HZ_PROFILE_FUNCTION();

	if (Hazel::Input::GetMouseDelta().x || Hazel::Input::GetMouseDelta().y) lastMoved = Hazel::Engine::GetTime();

	bool update = false;
	if (m_Paused)
	{
		if (m_Steps > 0)
		{
			update = true;
			m_Steps -= 1;
		}

	}
	else
	{
		update = true;
	}
	if(update)
		m_World->Update();
	else
		m_World->GetCamera().Update();

	if (m_DraggedBody && m_MouseDragged)
	{
		b2Vec2 velocity = {0.0f, 0.0f};
		if (Hazel::Input::DidMouseMove()) {
			glm::vec2 delta = { Hazel::Input::GetMouseDelta().x, Hazel::Input::GetMouseDelta().y };
			delta /= Hazel::Engine::GetDeltaTime();
			glm::vec2 result = m_World->GetCamera().GetWorldDelta(delta);
			velocity = { result.x, result.y };
		}
		
		m_DraggedBody->GetPhsicsBody()->SetLinearVelocity(velocity);
	}

}

void SandboxLayer::OnEvent(Hazel::Event* event)
{
	Hazel::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Hazel::MouseMovedEvent>(HZ_BIND_EVENT_FN(SandboxLayer::OnMouseMoved));
	dispatcher.Dispatch<Hazel::MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(SandboxLayer::OnMousePressed));
	dispatcher.Dispatch<Hazel::MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(SandboxLayer::OnMouseReleased));
}

bool SandboxLayer::OnMouseMoved(Hazel::MouseMovedEvent* event)
{
	if (m_DraggedBody)
		m_MouseDragged = true;
	return false;
}

template<typename T>
class QueryCallback : public b2QueryCallback
{
public:
	QueryCallback(b2Vec2 point, T func) : m_Func(func), m_Point(point) {}

	bool ReportFixture(b2Fixture* fixture)
	{
		if (fixture->TestPoint(m_Point))
		{
			m_Func(World::ToBody(fixture->GetBody()));
			return false;
		}
		else// The point doesnt actually overlap
		{
			return true;// Return true to continue the query.
		}

	}
private:
	T m_Func;
	b2Vec2 m_Point;
};


bool SandboxLayer::OnMousePressed(Hazel::MouseButtonPressedEvent* event)
{
	glm::vec2 worldCoords = m_World->GetCamera().ToWorldCoordinates(Hazel::Input::GetMousePosition());
	b2AABB aabb;
	aabb.lowerBound.Set(worldCoords.x, worldCoords.y);
	//aabb.upperBound.Set(worldCoords.x + 0.001f, worldCoords.y + 0.001f);
	aabb.upperBound.Set(worldCoords.x, worldCoords.y);
	bool found = false;
	auto callback = QueryCallback(b2Vec2(worldCoords.x, worldCoords.y), [this, &found](Body* body) {
		found = true;
		m_SelectedBody = body;
		m_DraggedBody = body;
		m_MouseDragged = false;
	});
	if (!found)
	{
		m_SelectedBody = nullptr;
		m_DraggedBody = nullptr;
		m_MouseDragged = false;
	}
	m_World->GetWorld()->QueryAABB(&callback, aabb);
	return false;
}

bool SandboxLayer::OnMouseReleased(Hazel::MouseButtonReleasedEvent* event)
{
	m_DraggedBody = nullptr;
	m_MouseDragged = false;
	return false;
}

static bool SliderDouble(const char* label, double* v, double v_min, double v_max, const char* format, float power)
{
	return ImGui::SliderScalar(label, ImGuiDataType_Double, v, &v_min, &v_max, format, power);
}

static void Tooltip(const char* text)
{
	if (ImGui::IsItemHovered() && (Hazel::Engine::GetTime() - lastMoved) > 0.05f)
	{
		ImGui::BeginTooltip();
		ImGui::TextUnformatted(text);
		ImGui::EndTooltip();
	}
}

int radioButtonCount = 0;
bool lastEnabled = true;

static void EndRadioButtons()
{
	if (!lastEnabled)
	{
		ImGui::PopStyleVar();
	}
	radioButtonCount = 0;
}

static void RadioButton(const char* title, int* var, int num)
{
	if (radioButtonCount > 0 && !lastEnabled) ImGui::PopStyleVar();

	lastEnabled = ImGui::RadioButton(title, var, num) || *var == num;
	
	if (!lastEnabled)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
	}

	radioButtonCount++;
	
}

float create_Angle = 0.0f, create_Radius = 1.0f, create_Width = 1.0f, create_Height = 1.0f;


enum CursorMode
{
	CreateEngine, CreateCapsule, CreateRocket, CreateSphere, CreateRect, Delete
};

CursorMode s_CurrentMode = CursorMode::CreateCapsule;

void SandboxLayer::OnImGuiRender()
{

	ImGui::Begin("World Settings");

	SliderDouble("G", &World::Constants::G, -0.01f, 10.0f, "%.5f", 3.0f); Tooltip("G is a universal constant like PI that determines how much objects attract each other");
	ImGui::Checkbox("Paused", &m_Paused); Tooltip("Pauses the simulation");
	int count = 0;
	for (b2Body* body = m_World->GetWorld()->GetBodyList(); body != nullptr; body = body->GetNext())
	{
		count++;
	}
	if (m_Paused)
	{
		if (ImGui::Button("Step"))
			m_Steps++;
	}


	ImGui::Text("%d bodies exist", count);
	if (ImGui::Button("Stop all bodies"))
	{
		m_World->ForEachBody([](b2Body* body) {	body->SetLinearVelocity({ 0.0f, 0.0f }); body->SetAngularVelocity(0.0f); });
	}
	if (ImGui::Button("Delete all bodies"))
	{
		m_World->ForEachBody([this](b2Body* body) { m_World->Remove(World::ToBody(body)); });
	}
	ImGui::End();

	ImGui::Begin("Selected Body");
	if (m_SelectedBody == nullptr)
	{
		ImGui::Text("No body is currently selected");
	}
	else
	{
		ImGui::Text("pos {%.2f, %.2f} m", m_SelectedBody->GetCenterOfMass().x, m_SelectedBody->GetCenterOfMass().y);
		ImGui::Text("rotation %.2f degrees", glm::degrees(m_SelectedBody->GetRotation()));
		ImGui::Text("mass %.2f kg", m_SelectedBody->GetPhsicsBody()->GetMass());
		ImGui::Text("speed %.3f m/s", m_SelectedBody->GetPhsicsBody()->GetLinearVelocity().Length());
		ImGui::Text("speed %.2f degrees/s", glm::degrees(m_SelectedBody->GetPhsicsBody()->GetAngularVelocity()));

		if (ImGui::Button("Delete"))
		{
			m_World->Remove(m_SelectedBody);
		}
		Ship* ship = dynamic_cast<Ship*>(m_SelectedBody);//ForEachPartIfType
		if (ship != nullptr)
		{
			if (ImGui::Button("Stage"))
			{
				int distance = -1;
				std::vector<Hazel::Ref<Part>> partsToStage;
				for (auto& part : ship->GetParts())
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
					Ship* newShip = ship->Split(*m_World, part);
					if (newShip) m_World->AddShip(newShip);
				}
			}
			
			if (ImGui::SliderFloat("Throttle", &ship->GetThrottle(), 0.0f, 1.0f))
			{
				ship->ForEachPartIfType<EnginePart>([ship](EnginePart& part) {
					part.SetThrottle(ship->GetThrottle());
				});
			}
		}
	}
	ImGui::End();

	ImGui::Begin("Cursor Modes");

	RadioButton("Create Capsule", reinterpret_cast<int*>(&s_CurrentMode), CursorMode::CreateCapsule);
	ImGui::TextUnformatted("Creates a new rocket astronaut capsule part when the mouse is clicked");
	ImGui::SliderAngle("Angle", &create_Angle, 0.0f);

	RadioButton("Create Engine", reinterpret_cast<int*>(&s_CurrentMode), CursorMode::CreateEngine);
	ImGui::TextUnformatted("Creates a new rocket engine part when the mouse is clicked");
	ImGui::SliderAngle("Angle", &create_Angle, 0.0f);
	
	RadioButton("Create Rocket", reinterpret_cast<int*>(&s_CurrentMode), CursorMode::CreateRocket);
	ImGui::TextUnformatted("Creates a new rocket when the mouse is clicked");
	ImGui::SliderAngle("Angle", &create_Angle, 0.0f);

	RadioButton("Create Sphere", reinterpret_cast<int*>(&s_CurrentMode), CursorMode::CreateSphere);
	ImGui::TextUnformatted("Creates a new sphere part when the mouse is clicked");
	ImGui::SliderAngle("Angle", &create_Angle, 0.0f);
	ImGui::SliderFloat("Radius", &create_Radius, 0.001f, 100.0f, "%0.3f", 2.0f);

	RadioButton("Create Rect", reinterpret_cast<int*>(&s_CurrentMode), CursorMode::CreateRect);
	ImGui::TextUnformatted("Creates a new rectangle part when the mouse is clicked");
	ImGui::SliderAngle("Angle", &create_Angle, 0.0f);
	ImGui::SliderFloat("Width", &create_Width, 0.001f, 100.0f, "%0.3f", 2.0f);
	ImGui::SliderFloat("Height", &create_Height, 0.001f, 100.0f, "%0.3f", 2.0f);

	RadioButton("Delete", reinterpret_cast<int*>(&s_CurrentMode), CursorMode::Delete);
	ImGui::TextUnformatted("Click an object to delete it");

	EndRadioButtons();
	ImGui::End();
}


void SandboxLayer::Render()
{
	m_World->Render();
}

SandboxLayer::~SandboxLayer()
{

}
