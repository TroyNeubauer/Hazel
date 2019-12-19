#pragma once

#include <Hazel.h>
#include <Box2D/Box2D.h>
#include <vector>

#include "Body.h"
#include "ship/Ship.h"
#include "Hazel/Camera/Camera.h"
#include "Hazel/Physics/Box2D/DebugRenderer.h"

struct LinkedListIterator
{
public:
	LinkedListIterator(b2Body* first) : m_Current(first) {}

	b2Body* operator* () { return m_Current; }
	b2Body* operator-> () { return m_Current; }

	LinkedListIterator& operator++ () { m_Current = m_Current->GetNext(); return *this; }
	LinkedListIterator operator++ (int) { const auto temp(*this); ++*this; return temp; }

	bool operator== (const LinkedListIterator& that) const { return m_Current == that.m_Current; }
	bool operator!= (const LinkedListIterator& that) const { return !(*this == that); }

	~LinkedListIterator() {}
private:
	b2Body* m_Current;

};

class World
{
public:
	World(Hazel::Camera2D& camera);

	struct Constants {
		const static double G;
	};
	static float GetAccelerationDueToGravity(float r, float mass);

	void Update(Hazel::Timestep ts);
	void Render();
	void RenderImpl(bool fullRender, const Hazel::Camera2D& camera);

	Hazel::Ref<Ship>& AddShip(const Hazel::Ref<EditorShip>& partDef, glm::vec2 pos, float rot = 0.0f);
	void AddShip(Ship* ship);//Takes ownership
	void Remove(Body* body);

	inline LinkedListIterator BodiesBegin() { return m_World->GetBodyList();  }
	inline LinkedListIterator BodiesEnd() { return nullptr; }

	inline const std::vector<Hazel::Ref<Ship>>& GetShips() const { return m_Ships; }
	inline b2World* GetWorld() { return m_World.get(); }

	inline void SetBodyRemovedCallback(std::function<void(Body*)> func) { m_OnRemovedFunction = func; }

	template<typename T>
	void ForEachBody(T callback)
	{
		for (b2Body* body = m_World->GetBodyList(); body != nullptr; )
		{
			b2Body* next = body->GetNext();//Cache next to allow for deleting bodies here
			callback(body);
			body = next;
		}
	}

	static inline Body* ToBody(b2Body* body) { return reinterpret_cast<Body*>(body->GetUserData()); }
	inline Hazel::B2D_DebugDraw* GetDebugDraw() { return m_DebugDraw.get(); }

private:
	std::vector<Hazel::Ref<Ship>> m_Ships;
	Hazel::Scope<b2World> m_World;
	Hazel::Camera2D& m_Camera;
	Hazel::Camera2D m_Camera2;
	std::function<void(Body*)> m_OnRemovedFunction;

	Hazel::Scope<Hazel::B2D_DebugDraw> m_DebugDraw;
	Hazel::Ref<Hazel::Shader> m_PlanetShader;
	Hazel::Ref<Hazel::FBO> m_MapFBO, m_DefaultFBO;
};


