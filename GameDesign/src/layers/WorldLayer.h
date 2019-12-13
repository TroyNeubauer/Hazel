#pragma once

#include <Hazel.h>
#include "../world/World.h"

class WorldLayer : public Hazel::Layer
{
public:
	inline WorldLayer(World* world) : Hazel::Layer("World Layer") { m_World.reset(world); }

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;

	virtual void OnEvent(Hazel::Event* event) override;
	virtual void OnImGuiRender() override;
	virtual void Render() override;

	virtual ~WorldLayer() override;
private:
	Hazel::Scope<World> m_World;

};
