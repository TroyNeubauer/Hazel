#include "SandboxLayer.h"

#include <imgui.h>
#include <random>

void SandboxLayer::OnAttach()
{
	std::default_random_engine gen;
	std::uniform_real_distribution<float> pos(-12.0f, 12.0f);
	Part& part = m_World->AddPart({ 0.0f, 0.0f }, Parts::StaticShip);

	for (int i = 0; i < 2; i++)
	{
		Part& part = m_World->AddPart({ pos(gen), pos(gen) }, Parts::MK1Capsule);
	}
	m_World->SetBodyRemovedCallback([this](Body* body) {
		if (body == m_SelectedBody) m_SelectedBody = nullptr;
		if (body == m_DraggedBody)
		{
			m_DraggedBody = nullptr;
			m_MouseDragged = false;
		}
		
	});
}

void SandboxLayer::OnDetach()
{

}

float lastMoved = 0.0f;

void SandboxLayer::OnUpdate()
{
	if (Hazel::Input::GetMouseDelta().x || Hazel::Input::GetMouseDelta().y) lastMoved = Hazel::Engine::GetTime();

	if (m_Paused)
		m_World->GetCamera().Update();
	else
		m_World->Update();

	if (m_DraggedBody && m_MouseDragged)
	{
		b2Vec2 velocity = {0.0f, 0.0f};
		if (Hazel::Input::DidMouseMove()) {
			glm::vec2 delta = { Hazel::Input::GetMouseDelta().x, Hazel::Input::GetMouseDelta().y };
			delta /= Hazel::Engine::GetDeltaTime();
			glm::vec2 result = m_World->GetCamera().GetWorldDelta(delta);
			velocity = { result.x, result.y };
		}
		
		m_DraggedBody->GetBody()->SetLinearVelocity(velocity);
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
	QueryCallback(T func) : m_Func(func) {}

	bool ReportFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();
		m_Func(World::ToBody(body));

		return false;// Return true to continue the query.
	}
private:
	T m_Func;
};

bool SandboxLayer::OnMousePressed(Hazel::MouseButtonPressedEvent* event)
{
	glm::vec2 worldCoords = m_World->GetCamera().ToWorldCoordinates(Hazel::Input::GetMousePosition());
	b2AABB aabb;
	aabb.lowerBound.Set(worldCoords.x, worldCoords.y);
	aabb.upperBound.Set(worldCoords.x + 0.001f, worldCoords.y + 0.001f);
	bool found = false;
	auto callback = QueryCallback([this, &found](Body* body) {
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
		ImGui::Text(text);
		ImGui::EndTooltip();
	}
}

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

	ImGui::Text("%d bodies exist", count);
	if (ImGui::Button("Stop all bodies"))
	{
		m_World->ForEachBody([](b2Body* body) {	body->SetLinearVelocity({ 0.0f, 0.0f }); });
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
		ImGui::Text("mass %.2f kg", m_SelectedBody->GetBody()->GetMass());
		ImGui::Text("speed %.3f m/s", m_SelectedBody->GetBody()->GetLinearVelocity().Length());
		ImGui::Text("speed %.2f rad/s", m_SelectedBody->GetBody()->GetAngularVelocity());

		if (ImGui::Button("Delete"))
		{
			m_World->Remove(m_SelectedBody);
		}
	}
	ImGui::End();
}


void SandboxLayer::Render()
{
	m_World->Render();
}

SandboxLayer::~SandboxLayer()
{

}
