#include "World.h"
#include "ship/Ship.h"
#include "Planet.h"
#include "ship/Part.h"

#include <map>
#include <glad/glad.h>

const float MARS_RADIUS = 33895.0f;//meters
const float MARS_GRAVITY = 3.711f;//meters/s2

const double World::Constants::G = 6.67430E-11;

World::World() : m_Camera(new WorldCameraController()), m_Camera2(nullptr)
{
	m_World.reset(new b2World( {0.0f, 0.0f} ));//No gravity since we handle it ourselves
	m_Camera.SetPosition(vec2(0.0, 0.0));
	m_Camera.SetRotation(0.0f);
	m_Camera.SetZoom(10.0f);

#if defined(HZ_DEBUG) && 1
	m_DebugDraw.reset(new Hazel::B2D_DebugDraw());
	m_DebugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
	m_World->SetDebugDraw(m_DebugDraw.get());
#endif
	
	m_PlanetShader = Hazel::Shader::Create("assets/shaders/Planet.glsl");
	
	Planet* planet = new Planet(*this, MARS_RADIUS, MARS_GRAVITY, glm::vec4(0.8f, 0.1f, 0.2f, 1.0f), glm::vec4(0.8f, 0.3f, 0.5f, 1.0f));
	planet->SetPosition({ 0.0f, -MARS_RADIUS - 10.0f });

	m_MapFBO = Hazel::FBO::Create(Hazel::Application::Get().GetWindow().GetWidth() / 4, Hazel::Application::Get().GetWindow().GetHeight() / 4);
	Hazel::TextureBuilder b = Hazel::TextureBuilder::Default();
	m_MapFBO->CreateColorBuffer(b);
	m_MapFBO->CreateDepthBuffer();
	m_MapFBO->Unbind();

	m_DefaultFBO = Hazel::FBO::GetDefault();
}

float World::GetAccelerationDueToGravity(float r, float mass)
{
	return Constants::G * mass / (r * r);
}

void World::Update(Hazel::Timestep ts)
{
	HZ_PROFILE_FUNCTION();

	float altitude = m_Camera.GetZoom();
	
	for (auto it = BodiesBegin(); it != BodiesEnd(); it++)
	{
		if (Planet* planet = dynamic_cast<Planet*>(ToBody(*it)))
		{
			glm::vec2 toShip = m_Camera.GetPosition() - planet->GetPosition();
			altitude = glm::length(toShip) - planet->GetRadius();
			m_Camera2.SetPosition(planet->GetPosition() + planet->GetRadius() * glm::normalize(toShip));
			m_Camera2.SetZoom(max(altitude + 60.0f, 10.0f));
			break;
		}
	}

	m_Camera.Update(ts);
	m_Camera2.Update(ts);
	{
		HZ_PROFILE_SCOPE("Box2D Gravity");

		b2Body* body = m_World->GetBodyList();
		for (int i = 0; i < m_World->GetBodyCount(); i++, body = body->GetNext())
		{
			b2Vec2 bodyPos = body->GetWorldCenter();

			b2Body* other = m_World->GetBodyList();
			for (int j = 0; j < i; j++, other = other->GetNext())
			{
				b2Vec2 otherPos = other->GetWorldCenter();
				// Determine the amount of force to give
				glm::dvec2 force = glm::dvec2(otherPos.x, otherPos.y) - glm::dvec2(bodyPos.x, bodyPos.y);
				double distance = glm::length(force);
				force = glm::normalize(force);
				force *= (World::Constants::G * (double) ToBody(body)->GetMass() * (double)ToBody(other)->GetMass()) / (distance * distance);
				b2Vec2 resultForce = b2Vec2(static_cast<float>(force.x), static_cast<float>(force.y));
				body->ApplyForceToCenter(resultForce, true);
				other->ApplyForceToCenter(-resultForce, true);
			}
		}
		body = m_World->GetBodyList();
		for (int i = 0; i < m_World->GetBodyCount(); i++, body = body->GetNext())
		{
			Body* myBody = ToBody(body);
			myBody->Update(ts, *this);
		}
	}
	{
		HZ_PROFILE_SCOPE("Box2D Step");
		m_World->Step(ts.Seconds(), 10, 10);
		m_World->ClearForces();
	}
	{
		HZ_PROFILE_SCOPE("Ships Update");
		for (auto& ship : m_Ships)
		{
			ship->Update(ts, *this);
		}
	}
}

void World::Render()
{
	HZ_PROFILE_FUNCTION();

	m_MapFBO->Bind();
	Hazel::RenderCommand::SetClearColor( {0.0f, 1.0f, 1.0f, 1.0f} );
	Hazel::RenderCommand::Clear();
	RenderImpl(false, m_Camera2);

	m_DefaultFBO->Bind();
	RenderImpl(true, m_Camera);
	Hazel::PostprocessingEffects::CopyToScreen(m_MapFBO, { 0.5f, -1.0f }, { 1.0f, -0.5 });
}

void World::RenderImpl(bool fullRender, const Hazel::Camera2D& camera)
{
	if (m_DebugDraw && fullRender)
	{
		HZ_PROFILE_SCOPE("Box2D Debug Draw");
		m_DebugDraw->BeginScene(camera);
		m_World->DrawDebugData();
		for (const auto& ship : m_Ships)
		{
			ship->B2DRender(m_DebugDraw.get());
		}
		m_DebugDraw->EndScene();
	}

	{
		HZ_PROFILE_SCOPE("Main Draw");
		//Render the planets first since they arent batched
		for (auto it = BodiesBegin(); it != BodiesEnd(); it++)
		{
			Body* body = ToBody(*it);
			if (Planet* planet = dynamic_cast<Planet*>(body))
			{
				planet->Render(camera, m_PlanetShader);
			}
		}
		//Render everything else
		Hazel::Renderer2D::BeginScene(camera);
		for (auto it = BodiesBegin(); it != BodiesEnd(); it++)
		{
			Body* body = ToBody(*it);
			if (!dynamic_cast<Planet*>(body)) body->Render(camera);
		}

		if (!fullRender) Hazel::Renderer2D::DrawQuad(m_Camera.ToWorldCoordinates(Hazel::Input::GetMousePosition()), { 0.1f, 0.1f }, 0xFF00FFFF);
		Hazel::Renderer2D::EndScene();
	}
	if (fullRender)
	{
		HZ_PROFILE_SCOPE("Particles Draw");
		Hazel::Renderer2D::BeginScene(camera);
		for (auto it = BodiesBegin(); it != BodiesEnd(); it++)
		{
			Ship* ship = dynamic_cast<Ship*>(ToBody(*it));
			if (ship) ship->RenderParticles(camera);
		}
		Hazel::Renderer2D::EndScene();
	}
}

void World::AddShip(Ship* ship)
{
	m_Ships.push_back(Hazel::S(ship));
}

Ship& World::AddShip(const Hazel::Ref<EditorShip>& shipDef, glm::vec2 pos, float rot)
{
	HZ_PROFILE_FUNCTION();

	Ship* ship = new Ship(*this, shipDef, pos, rot);
	m_Ships.push_back(Hazel::S(ship));
	return *ship;
}

void World::Remove(Body* body)
{
	HZ_PROFILE_FUNCTION();

	if(m_OnRemovedFunction)
		m_OnRemovedFunction(body);

	m_World->DestroyBody(body->GetPhsicsBody());
	for (auto it = m_Ships.begin(); it != m_Ships.end(); ++it)
	{
		Hazel::Scope<Ship>& ship = *it;
		if (ship->GetPhsicsBody() == body->GetPhsicsBody())
		{
			m_Ships.erase(it);
			break;
		}
	}
}


const float ACCEL_SPEED = 25.0f;

void WorldCameraController::Update(Hazel::Timestep ts, Hazel::Camera2D& camera)
{
	HZ_PROFILE_FUNCTION();

	glm::vec2 move = { 0, 0 };
	glm::vec2 right = glm::rotate(glm::vec2(1.0f, 0.0f), glm::radians(camera.m_Rot));
	glm::vec2 up = glm::rotate(glm::vec2(0.0f, 1.0f), glm::radians(camera.m_Rot));
	bool moving = false;

	if (Hazel::Input::IsKeyPressed(HZ_KEY_W)) {
		move += up; moving = true;
	} if (Hazel::Input::IsKeyPressed(HZ_KEY_S)) {
		move -= up; moving = true;
	} if (Hazel::Input::IsKeyPressed(HZ_KEY_D)) {
		move += right; moving = true;
	} if (Hazel::Input::IsKeyPressed(HZ_KEY_A)) {
		move -= right; moving = true;
	}
	float length = glm::length(move);
	if (length > 0.0f)
	{
		move /= length;
		move *= ACCEL_SPEED;
	}

	camera.m_ZoomVel -= Hazel::Input::GetScrollDelta() * 3.0f;

	camera.m_Vel += move * ts.Seconds();
	camera.m_Pos += camera.m_Vel * ts.Seconds();
	if (!moving)
	{
		camera.m_Vel *= (1.0f - ts.Seconds() * 2.0f);
	}

	camera.m_Zoom += camera.m_ZoomVel * ts.Seconds();
	camera.m_ZoomVel *= (1.0f - ts.Seconds() * 5.0f);

	if (camera.m_Zoom <= 0.00001f)
		camera.m_Zoom = 0.00001f;

}
