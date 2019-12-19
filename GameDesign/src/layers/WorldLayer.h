#pragma once

#include <Hazel.h>
#include "../world/World.h"

class WorldLayer : public Hazel::Layer
{
public:
	WorldLayer(Hazel::Ref<EditorShip>& originShip);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;

	virtual void OnEvent(Hazel::Event* event) override;
	virtual void OnImGuiRender() override;
	virtual void Render() override;

	virtual ~WorldLayer() override;
private:
	Hazel::Scope<World> m_World;
	Hazel::Ref<Ship> m_Ship;
	Hazel::Camera2D m_Camera;

};

class WorldCameraController : public Hazel::CameraController2D
{
	friend class Hazel::Camera2D;
	virtual void Update(Hazel::Timestep ts, Hazel::Camera2D& camera) override;
};

