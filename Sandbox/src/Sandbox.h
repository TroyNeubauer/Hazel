#pragma once

#include <Hazel.h>
#include <imgui/imgui.h>

#include "Terrain.h"

#include <vector>

extern bool launching;
extern float launchTime;

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
	std::shared_ptr<Hazel::FPSCamera> m_Camera;
	Terrain* m_terrain;
	Terrain* m_terrain2;

	std::vector<std::shared_ptr<Hazel::Mesh>> m_Meshes;
	std::vector<Hazel::Light> m_Lights;

};

extern Sandbox* sandbox;

Hazel::Application* Hazel::CreateApplication(int argc, char** argv);