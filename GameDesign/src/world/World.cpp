#include "World.h"
#include "ship/Ship.h"
#include "Planet.h"
#include "ship/Part.h"

#include <map>


double World::Constants::G = 6.67430E-11 * 10000000.0f;

World::World() : m_Camera(new WorldCameraController())
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
}

void World::Update()
{
	HZ_PROFILE_FUNCTION();

	m_Camera.Update();
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

				glm::dvec2 force = glm::dvec2(otherPos.x, otherPos.y) - glm::dvec2(bodyPos.x, bodyPos.y);
				force = glm::normalize(force);
				double distance = glm::length(force);// Determine the amount of force to give
				force *= (World::Constants::G * (double)body->GetMass() * (double)other->GetMass()) / (distance * distance);
				b2Vec2 resultForce = b2Vec2(static_cast<float>(force.x), static_cast<float>(force.y));
				body->ApplyForceToCenter(resultForce, true);
				other->ApplyForceToCenter(-resultForce, true);
			}
		}
		body = m_World->GetBodyList();
		for (int i = 0; i < m_World->GetBodyCount(); i++, body = body->GetNext())
		{
			Body* myBody = ToBody(body);
			myBody->Update(*this);
		}
	}
	{
		HZ_PROFILE_SCOPE("Box2D Step");
		m_World->Step(Hazel::Engine::GetDeltaTime(), 10, 10);
		m_World->ClearForces();
	}
}

void World::Render()
{
	HZ_PROFILE_FUNCTION();

	if (m_DebugDraw)
	{
		HZ_PROFILE_SCOPE("Box2D Debug Draw");
		m_DebugDraw->BeginScene(m_Camera);
		m_World->DrawDebugData();
		for (const auto& ship : m_Ships)
		{
			ship->B2DRender(m_DebugDraw.get());
		}
		m_DebugDraw->EndScene();
	}

	{
		HZ_PROFILE_SCOPE("Main Draw");
		Hazel::Renderer2D::BeginScene(m_Camera);
		for (auto it = BodiesBegin(); it != BodiesEnd(); it++)
		{
			Body* body = ToBody(*it);
			body->Render(*this);
		}
		Hazel::Renderer2D::DrawQuad(m_Camera.ToWorldCoordinates(Hazel::Input::GetMousePosition()), { 0.1f, 0.1f }, 0xFF00FFFF);
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

void WorldCameraController::Update(Hazel::Camera2D& camera)
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
	float length = move.length();
	if (length > 0.0f)
	{
		move /= length;
		move *= ACCEL_SPEED;
	}

	camera.m_ZoomVel -= Hazel::Input::GetScrollDelta() * 3.0f;

	camera.m_Vel += move * Hazel::Engine::GetDeltaTime();
	camera.m_Pos += camera.m_Vel * Hazel::Engine::GetDeltaTime();
	if (!moving)
	{
		camera.m_Vel *= (1.0f - Hazel::Engine::GetDeltaTime() * 2.0f);
	}

	camera.m_Zoom += camera.m_ZoomVel * Hazel::Engine::GetDeltaTime();
	camera.m_ZoomVel *= (1.0f - Hazel::Engine::GetDeltaTime() * 5.0f);

	if (camera.m_Zoom <= 0.00001f)
		camera.m_Zoom = 0.00001f;

}
