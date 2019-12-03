#pragma once

#include <Hazel.h>
#include <Box2D/Box2D.h>
#include <vector>

#include "Body.h"
#include "ship/Ship.h"
#include "Hazel/Camera/Camera.h"


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


class Parts {
public:
	static Hazel::Ref<PartDef> MK1Capsule;
	static Hazel::Ref <PartDef> FlyingShip;
	static Hazel::Ref <PartDef> StaticShip;
};

class World
{
public:
	World();

	struct Constants {
		static double G;
	};

	void Update();
	void Render();

	Part& AddPart(b2Vec2 pos, const Hazel::Ref<PartDef>& partDef, float rot = 0.0f);
	void Remove(Body* body);

	inline LinkedListIterator BodiesBegin() { return m_World->GetBodyList();  }
	inline LinkedListIterator BodiesEnd() { return nullptr; }

	inline b2World* GetWorld() { return m_World.get(); }
	inline Hazel::Camera2D& GetCamera() { return m_Camera; }

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

private:
	std::vector<Hazel::Scope<Ship>> m_Ships;
	Hazel::Scope<b2World> m_World;
	Hazel::Camera2D m_Camera;
	std::function<void(Body*)> m_OnRemovedFunction;
};

class WorldCameraController : public Hazel::CameraController2D
{
	friend class Camera2D;
	virtual void Update(Hazel::Camera2D& camera);
};


