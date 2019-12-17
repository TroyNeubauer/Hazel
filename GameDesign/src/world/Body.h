#pragma once

#include <Box2D/Box2D.h>
#include <Hazel/Core/Core.h>
#include <Hazel/Core/glm.h>
#include <Hazel/Camera/Camera.h>

class World;

//Represents a body in a particular world
class Body
{
public:
	//m_Body must be initalized in the child's constructor
	Body() : m_Body(nullptr) {}

	inline b2Body* GetPhsicsBody() const { return m_Body; }
	
	void SetPosition(glm::vec2 pos);
	glm::vec2 GetPosition() const;

	//Returns the angle in degrees
	float GetRotation() const;
	void SetRotation(float radians);
	virtual float GetMass() const;

	glm::vec2 GetLocalCenterOfMass() const;
	inline glm::vec2 GetCenterOfMass() const { return GetPosition() + GetLocalCenterOfMass(); }

	virtual void Render(const Hazel::Camera& camera) = 0;
	virtual void Update(Hazel::Timestep ts, World& world) = 0;

	virtual ~Body() {}

protected:
	void SetPhysicsBody(b2Body* newBody);

private:
	b2Body* m_Body;

};
