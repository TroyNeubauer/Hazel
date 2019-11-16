#pragma once

#include <Hazel.h>


class WorldLayer : public Hazel::Layer
{
	inline WorldLayer() : Hazel::Layer("World Layer") {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

	virtual void OnEvent(Hazel::Event* event) override;
	virtual void OnImGuiRender() override;

	virtual ~WorldLayer() override;


};
