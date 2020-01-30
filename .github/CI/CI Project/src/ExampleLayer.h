#pragma once

#include <Hazel.h>


class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	virtual void OnUpdate(Hazel::Timestep ts) override
	{

	}

	virtual void OnImGuiRender() override
	{
		
	}

	virtual void OnEvent(Hazel::Event* event) override
	{
	}

	bool KeyReleased(Hazel::KeyReleasedEvent* event)
	{
		return false;
	}

	bool KeyPressed(Hazel::KeyPressedEvent* event)
	{

		return false;
	}

};
