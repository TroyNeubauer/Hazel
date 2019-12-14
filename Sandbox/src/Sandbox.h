#pragma once

#include <Hazel.h>
#include <imgui/imgui.h>

#include "Terrain.h"

#include <vector>


class Sandbox : public Hazel::Application
{
public:
	Sandbox();

	virtual void Update(Hazel::Timestep ts) override;
	virtual void Render() override;

	~Sandbox();

private:
	friend class ExampleLayer;
	Hazel::Ref<Hazel::FPSCamera> m_Camera;
	Terrain* m_terrain;
	Hazel::Ref<Hazel::Shader> m_PBRShader;

	
public:
	std::vector<Hazel::Ref<Hazel::Mesh>> m_Meshes;
	bool Paused = false;
	std::vector<Hazel::Light> m_Lights;

};

extern Sandbox* sandbox;

Hazel::Application* Hazel::CreateApplication(int argc, char** argv);
