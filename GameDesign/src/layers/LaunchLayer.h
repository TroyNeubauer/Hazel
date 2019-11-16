#pragma once

#include <Hazel.h>


class LaunchLayer : public Hazel::Layer
{
	inline LaunchLayer() : Hazel::Layer("Launch Layer") {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

	virtual void OnEvent(Hazel::Event* event) override;
	virtual void OnImGuiRender() override;

	virtual ~LaunchLayer() override;


};
