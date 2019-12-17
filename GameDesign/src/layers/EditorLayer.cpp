#include "EditorLayer.h"

#include "ship/Parts.h"

#include <imgui.h>
#include <Hazel.h>

EditorLayer::EditorLayer() : m_Camera(nullptr), m_UICamera(nullptr), m_ActiveShip(new EditorShip())
{
	m_Camera.SetZoom(100.0f);
	m_UICamera.SetZoom(1.0f);

	m_Camera.SetPosition({ 0.0f, 0.0f });
	m_UICamera.SetPosition({ 0.0f, 0.0f });

}

void EditorLayer::OnAttach()
{
}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(Hazel::Timestep)
{
	m_Camera.ForceUpdate();
	m_UICamera.ForceUpdate();

}

void EditorLayer::OnEvent(Hazel::Event * event)
{
}

static bool Intersects(glm::vec2 point, glm::vec2 rectCenter, glm::vec2 size, float angle)
{
	if (angle > 45.0f && angle < 135.0f)
		std::swap(size.x, size.y);
	if (angle > 225.0f && angle < 315.0f)
		std::swap(size.x, size.y);
	glm::vec2 bottomLeft = rectCenter - size / 2.0f;
	glm::vec2 topRight = rectCenter + size / 2.0f;

	return point.x >= bottomLeft.x && point.y >= bottomLeft.y && point.x <= topRight.x && point.y <= topRight.y;
}

Hazel::Ref<EditorPart> EditorLayer::Raycast(glm::ivec2 cursorPos)
{
	glm::vec2 worldPos = m_Camera.ToWorldCoordinates(cursorPos);
	for (auto& part : m_ActiveShip->GetParts())
	{
		if (Intersects(worldPos, part->GetTotalOffset(0.0f), part->m_Def->HitboxSize, part->GetTotalRotation()))
		{

		}
	}
	for (auto& part : m_DetachedParts)
	{

	}


	return nullptr;
}

bool EditorLayer::OnMouseMoved(Hazel::MouseMovedEvent* event)
{
	return false;
}

bool EditorLayer::OnMousePressed(Hazel::MouseButtonPressedEvent* event)
{
	return false;
}

bool EditorLayer::OnMouseReleased(Hazel::MouseButtonReleasedEvent* event)
{
	return false;
}

void EditorLayer::OnImGuiRender()
{
	if (m_HoveredShop)
	{
		ImGui::BeginTooltip();
		ImGui::TextUnformatted("Test");
		ImGui::TextUnformatted(m_HoveredShop->Name);
		ImGui::EndTooltip();
	}
}

void EditorLayer::Render()
{
	Hazel::Renderer2D::BeginScene(m_UICamera);

	RenderPartsList();

	Hazel::Renderer2D::DrawQuad(m_UICamera.ToWorldCoordinates(Hazel::Input::GetMousePosition()), { 0.01f, 0.01f }, 0xFF00FFFF);

	Hazel::Renderer2D::EndScene();

	//Render parts in editor
	Hazel::Renderer2D::BeginScene(m_Camera);
	Hazel::Renderer2D::DrawQuad(m_Camera.ToWorldCoordinates(Hazel::Input::GetMousePosition()), { 1, 1 }, 0x00FFFFFF);

	for (auto& part : m_ActiveShip->GetParts())
	{
		part->Render(m_Camera);
	}
	for (auto& part : m_DetachedParts)
	{
		part->Render(m_Camera);
	}

	if (m_DraggedReal)
	{
		m_DraggedReal->Render(m_Camera);
	}
	Hazel::Renderer2D::EndScene();
}


Hazel::Ref<EditorPart> EditorLayer::RenderPartsList()
{
	float aspect = static_cast<float>(Hazel::Application::Get().GetWindow().GetWidth()) / static_cast<float>(Hazel::Application::Get().GetWindow().GetHeight());
	
	const float padding = 0.02f;
	glm::vec2 cursorStart = { -m_UICamera.GetZoom() * aspect + padding, m_UICamera.GetZoom() - padding };
	glm::vec2 cursor = cursorStart;

	glm::vec2 realCursorPos = m_UICamera.ToWorldCoordinates(Hazel::Input::GetMousePosition());

	glm::vec2 partSize = { 0.1f, 0.1f };
	float maxY = 0.0f;

	m_HoveredShop = nullptr;
	for (auto& part : Parts::AllParts)
	{
		Hazel::Renderer2D::Renderable renderable;
		glm::vec2 individualPartSize = partSize;
		individualPartSize.y /= part->Animation->AspectRatio();
		if (individualPartSize.y > maxY) maxY = individualPartSize.y;
		renderable.Position = { cursor + glm::vec2(individualPartSize.x / 2.0f, -individualPartSize.y / 2.0f), 1.0f };
		renderable.Size = individualPartSize;
		renderable.ApplyAnimation(part->Animation, part->Animation->m_Frames[0]);

		if (Intersects(realCursorPos, renderable.Position, renderable.Size, 0.0f))
		{
			m_HoveredShop = part;
		}

		Hazel::Renderer2D::DrawQuad(renderable);
		cursor.x += individualPartSize.x + padding;
		if (cursor.x >= -0.8f * aspect)
		{
			cursor.x = cursorStart.x;
			cursor.y -= maxY + padding;
		}
			
	}

	return Hazel::Ref<EditorPart>();
}


EditorLayer::~EditorLayer()
{
}
