#pragma once

#include <Hazel.h>
#include <imgui/imgui.h>

#include <vector>


class Sandbox : public Hazel::Application
{
public:
	Sandbox();

	virtual void Update(Hazel::Timestep ts) override;
	virtual void Render() override;

	~Sandbox();

};

