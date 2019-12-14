#include "hzpch.h"
#include "Ship.h"
#include "Part.h"
#include "world/World.h"
#include "Helper.h"

#include <Hazel.h>
#include <map>

EditorShip::EditorShip(const EditorShip& other)
{
	//Maps other parts to new parts
	std::unordered_map<Hazel::Ref<EditorPart>, Hazel::Ref<EditorPart>> partLinking(other.m_Parts.size());
	for (int i = 0; i < other.m_Parts.size(); i++)
	{
		const Hazel::Ref<EditorPart>& otherPart = other.m_Parts[i];
		Hazel::Ref<EditorPart>& part = m_Parts.emplace_back(new EditorPart(otherPart));
		partLinking[otherPart] = part;
	}

	for (int i = 0; i < other.m_Parts.size(); i++)
	{
		const Hazel::Ref<EditorPart>& otherPart = other.m_Parts[i];
		Hazel::Ref<EditorPart>& part = m_Parts[i];
		if (otherPart->m_ParentPart != nullptr)
		{
			std::unordered_map<Hazel::Ref<EditorPart>, Hazel::Ref<EditorPart>>::iterator it = partLinking.find(otherPart);
			HZ_ASSERT(it != partLinking.end(), "Invalid parent part is not on this ship!");
			Hazel::Ref<EditorPart>& parentPart = it->second;
			part->m_ParentPart = parentPart;
		}

	}
}

Ship::Ship(World& world, const Hazel::Ref<EditorShip>& shipDef, glm::vec2 pos, float degrees)
{
	std::unordered_map<Hazel::Ref<EditorPart>, Hazel::Ref<Part>> partLinking(shipDef->GetParts().size());
	for (const Hazel::Ref<EditorPart>& partDef : shipDef->GetParts())
	{
		Hazel::Ref<Part>& part = m_Parts.emplace_back(partDef->m_Def->CreatePart(world, *this, partDef));
		partLinking[partDef] = part;
	}


	for (Hazel::Ref<Part>& part : m_Parts)
	{
		if (part->m_EditorPart->m_ParentPart != nullptr)
		{
			std::unordered_map<Hazel::Ref<EditorPart>, Hazel::Ref<Part>>::iterator it = partLinking.find(part->m_EditorPart->m_ParentPart);
			HZ_ASSERT(it != partLinking.end(), "Invalid parent part is not on this ship!");
			Hazel::Ref<Part>& parentPart = it->second;
			part->m_ParentPart = parentPart;
		}

	}
	CreatePhysicsBody(world, pos, degrees);
}

void Ship::Render(World& world)
{
	glm::vec2 rootPos = { GetPhsicsBody()->GetPosition().x, GetPhsicsBody()->GetPosition().y };
	for (Hazel::Ref<Part>& part : m_Parts)
	{
		part->Render(world, *this);
	}
}

void Ship::Update(Hazel::Timestep ts, World& world)
{
	for (Hazel::Ref<Part>& part : m_Parts)
	{
		part->Update(ts, world, *this);
	}
}

void Ship::B2DRender(Hazel::B2D_DebugDraw* draw)
{
	for (size_t i = 0; i < m_Parts.size(); i++)
	{
		Hazel::Ref<Part>& part = m_Parts[i];
		glm::vec2 a = GetPosition() + part->GetTotalOffset(GetRotation());
		glm::vec2 b = a + part->GetAngularVelocityAsLinear(*this);
		draw->DrawSegment(v2b2(a), v2b2(b), b2Color(1.0f, 0.0f, 1.0f, 1.0f));
	}
}


Ship* Ship::Split(World& world, Hazel::Ref<Part>& newRoot)
{
	HZ_ASSERT(&GetRoot() != newRoot.get(), "Cannot split the root part!");
	Ship* result = new Ship();
	std::vector<size_t> newPartsIndices;
	for (size_t i = 0; i < m_Parts.size(); i++)
	{
		Hazel::Ref<Part>& part = m_Parts[i];
		if (part.get() == newRoot.get())
		{
			newPartsIndices.push_back(i);
			continue;
		}

		Part* parent = part->m_ParentPart.get();
		while (parent)
		{
			if (parent == newRoot.get())
			{
				//Part is a child of newRoot
				newPartsIndices.push_back(i);
				break;
			}

			parent = parent->m_ParentPart.get();
		}
	}
	if (newPartsIndices.empty()) return nullptr;//No work to do

	std::sort(newPartsIndices.begin(), newPartsIndices.end());

	result->m_Parts.resize(newPartsIndices.size());
	int i = newPartsIndices.size() - 1;

	float rot = GetRotation();
	glm::vec2 pos = GetPosition();
	glm::vec2 offset = newRoot->GetTotalOffset(rot);
	pos += offset;
	glm::vec2 velocity = newRoot->GetAngularVelocityAsLinear(*this) + b2v2(GetPhsicsBody()->GetLinearVelocity());
	float rotation = newRoot->GetTotalRotation() + GetRotation();

	//Iterate and remove parts in from the higher indices first to minimize copying
	for (auto it = newPartsIndices.rbegin(); it != newPartsIndices.rend(); it++)
	{
		size_t index = *it;
		Hazel::Ref<Part>& origionalPart = m_Parts[index];
		Hazel::Ref<Part>& newPart = result->m_Parts[i];
		//Add the fixtures of this part to the new body
		origionalPart->RemoveFixtures(GetPhsicsBody());

		//Transfer ownership to the new part
		newPart = origionalPart;
		//Remove the part from the old ship's list
		m_Parts.erase(m_Parts.begin() + index);

		if (newPart.get() == newRoot.get())
		{
			result->m_RootIndex = i;
			newPart->m_ParentPart = nullptr;//Disconnect from the main ship
		}

		i--;
	}

	HZ_ASSERT(i == -1, "Did not iterate properly!");
	HZ_ASSERT(result->m_RootIndex != -1, "No root found on new body");
	HZ_ASSERT(result->GetRoot().m_ParentPart == nullptr, "Root is not a real root!");

	//Create the new physics body
	result->CreatePhysicsBody(world, pos, rot);//CreatePhysicsBody adds the fixtures for each part
	result->GetPhsicsBody()->SetLinearVelocity(v2b2(velocity));
	result->SetRotation(rotation);

	return result;
}

void Ship::CreatePhysicsBody(World& world, glm::vec2 pos, float radians)
{
	b2BodyDef def;
	def.position = b2Vec2(pos.x, pos.y);
	def.angle = radians;
	def.userData = this;
	def.type = b2_dynamicBody;
	def.active = true;
	b2Body* body = world.GetWorld()->CreateBody(&def);
	SetPhysicsBody(body);

	for (Hazel::Ref<Part>& part : m_Parts)
	{
		part->AddFixtures(body);
	}
}


Part& Ship::GetRoot()
{
	if (m_RootIndex == -1)
	{
		int i;
		for (i = 0; i < m_Parts.size(); i++)
		{
			if (m_Parts[i]->m_ParentPart == nullptr)
			{
				m_RootIndex = i;
				goto end;
			}
		}
		HZ_ASSERT(false, "Failed to find a root part!");
	}
	end:
	return *m_Parts[m_RootIndex].get();
}

Ship::~Ship()
{
	for (Hazel::Ref<Part>& part : m_Parts)
	{
		//Unlink parents so that that each part can be freed when the vector gets destructed
		part->m_ParentPart = nullptr;
	}
}
