#pragma once

#include <Box2D/Box2D.h>
#include <Hazel/glm.h>

class World;

class Body
{
public:
	Body() {}

	inline b2Body* GetBody() { return m_Body; }
	
	inline glm::vec2 GetPosition() { return { m_Body->GetPosition().x, m_Body->GetPosition().y }; }
	
	//Returns the angle in radains
	inline float GetRotation() { return m_Body->GetAngle(); }

	void ApplyGravity(const World& world);

	virtual void Render(const World& world) = 0;
	virtual void Update(const World& world) = 0;


protected:
	b2Body* m_Body;

};