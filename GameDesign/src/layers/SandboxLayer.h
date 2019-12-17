#pragma once

#include <Hazel.h>
#include "../world/World.h"

class SandboxLayer : public Hazel::Layer
{
public:
	SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep) override;

	virtual void OnEvent(Hazel::Event* event) override;

	bool OnMouseMoved(Hazel::MouseMovedEvent* event);
	bool OnMousePressed(Hazel::MouseButtonPressedEvent* event);
	bool OnMouseReleased(Hazel::MouseButtonReleasedEvent* event);

	virtual void OnImGuiRender() override;
	virtual void Render() override;

	virtual ~SandboxLayer() override;

private:
	Hazel::Scope<World> m_World;
	Hazel::Camera2D m_Camera;
	Body* m_SelectedBody = nullptr;
	Body* m_DraggedBody = nullptr; bool m_MouseDragged = false;
	bool m_Paused = false;
	int m_Steps = -1;
};

class SandboxCameraController : public Hazel::CameraController2D
{
	friend class Hazel::Camera2D;
	virtual void Update(Hazel::Timestep ts, Hazel::Camera2D& camera) override;
};
