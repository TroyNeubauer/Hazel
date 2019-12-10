#pragma once

#include <Hazel.h>
#include "../world/World.h"

class SandboxLayer : public Hazel::Layer
{
public:
	inline SandboxLayer(World* world) : Hazel::Layer("Sandbox Layer") { m_World.reset(world); }

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

	virtual void OnEvent(Hazel::Event* event) override;

	bool OnMouseMoved(Hazel::MouseMovedEvent* event);
	bool OnMousePressed(Hazel::MouseButtonPressedEvent* event);
	bool OnMouseReleased(Hazel::MouseButtonReleasedEvent* event);

	virtual void OnImGuiRender() override;
	virtual void Render() override;

	virtual ~SandboxLayer() override;

private:
	Hazel::Scope<World> m_World;
	Body* m_SelectedBody = nullptr;
	Body* m_DraggedBody = nullptr; bool m_MouseDragged = false;
	bool m_Paused = true;
	int m_Steps = -1;
};
