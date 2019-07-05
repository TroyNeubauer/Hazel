#pragma once

#include <Hazel.h>
#include <imgui/imgui.h>

#include "Terrain.h"

extern bool launching;
extern float launchTime;

extern vec3 lightPosition, lightColor;
extern bool paused, generating;

float GetTime();

float GetHeight();

class Sandbox : public Hazel::Application
{
public:
	Sandbox();

	virtual void Update() override;

	virtual void Render() override;

	~Sandbox();

private:
	friend class ExampleLayer;
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::FPSCamera> m_Camera;
	std::shared_ptr<Hazel::Texture2D> m_GrassTexture, m_TargetTexture;
	Terrain* m_terrain;
};

extern Sandbox* sandbox;

Hazel::Application* Hazel::CreateApplication(int argc, char** argv);