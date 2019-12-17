#pragma once

#include "ship/Ship.h"
#include "ship/Part.h"

#include <Hazel.h>

class EditorLayer : public Hazel::Layer
{
public:
	EditorLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep) override;

	virtual void OnEvent(Hazel::Event* event) override;
	Hazel::Ref<EditorPart> Raycast();

	bool OnMouseMoved(Hazel::MouseMovedEvent* event);
	bool OnMousePressed(Hazel::MouseButtonPressedEvent* event);
	bool OnMouseReleased(Hazel::MouseButtonReleasedEvent* event);

	virtual void OnImGuiRender() override;
	virtual void Render() override;

	virtual ~EditorLayer() override;

private:
	void RenderPartsList();

private:
	Hazel::Ref<EditorShip> m_ActiveShip;
	std::vector<Hazel::Ref<EditorPart>> m_DetachedParts;
	Hazel::Camera2D m_Camera, m_UICamera;

	Hazel::Ref<PartDef> m_HoveredShop;
	Hazel::Ref<EditorPart> m_HoveredReal;
	Hazel::Ref<EditorPart> m_HeldPart;
	Hazel::Ref<EditorPart> m_GohstPlace, m_GohstParent;
	int gohstSize;


};
