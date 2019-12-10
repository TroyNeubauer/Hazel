#include "hzpch.h"
#include "Ship.h"
#include "Part.h"
#include "world/World.h"

#include <map>

Ship::Ship(World& world, const Hazel::Ref<EditorShip>& shipDef, glm::vec2 pos, float rot)
{
	std::unordered_map<Hazel::Ref<EditorPart>, Hazel::Ref<Part>> partLinking(shipDef->GetParts().size());
	for (Hazel::Ref<EditorPart>& partDef : shipDef->GetParts())
	{
		Hazel::Ref<Part>& part = m_Parts.emplace_back(new Part(world, *this, partDef));
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
	CreatePhysicsBody(world, pos, rot);
}

void Ship::Render(World& world)
{
	glm::vec2 rootPos = { GetPhsicsBody()->GetPosition().x, GetPhsicsBody()->GetPosition().y };
	for (Hazel::Ref<Part>& part : m_Parts)
	{
		float angle = GetPhsicsBody()->GetAngle() + part->GetEditorPart()->GetTotalRotation();
		glm::vec2 pos = rootPos + part->GetEditorPart()->GetTotalOffset(GetPhsicsBody()->GetAngle());
		glm::vec2 size = { part->GetEditorPart()->m_Def->Size.x, part->GetEditorPart()->m_Def->Size.y };
		Hazel::Renderer2D::DrawQuad(pos, size, part->m_Animation, glm::degrees(angle));
	}
}

void Ship::Update(World& world)
{
	for (Hazel::Ref<Part>& part : m_Parts)
	{
		part->Update(world);
	}
}


Ship* Ship::Split(World& world, Hazel::Ref<Part> newRoot)
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

	float rot = GetRotation();
	glm::vec2 pos = GetPosition() + newRoot->m_EditorPart->GetTotalOffset(rot);

	result->CreatePhysicsBody(world, pos, rot);
	b2Body* resultBody = result->GetPhsicsBody();

	//Make newPartsIndices be in descending order
	std::sort(newPartsIndices.begin(), newPartsIndices.end());
	std::reverse(newPartsIndices.begin(), newPartsIndices.end());

	result->m_Parts.resize(newPartsIndices.size());
	int i = newPartsIndices.size() - 1;
	for (auto it = newPartsIndices.rbegin(); it != newPartsIndices.rend(); it++)
	{
		size_t index = *it;
		Hazel::Ref<Part>& origionalPart = m_Parts[index];
		Hazel::Ref<Part>& newPart = result->m_Parts[i];
		origionalPart->RemoveFixtures(GetPhsicsBody());

		//Transfer ownership to the new part
		newPart = origionalPart;
		//Remove the part from the old ship's list
		m_Parts.erase(m_Parts.begin() + index);

		//Add the fixtures of this part to the new body
		newPart->AddFixtures(resultBody);
		if (newPart.get() == newRoot)
		{
			result->m_RootIndex = i;
			newPart->m_ParentPart = nullptr;//Disconnect from the main ship
		}

		i--;

	}
	HZ_ASSERT(i == -1, "Did not iterate properly!");
	HZ_ASSERT(result->m_RootIndex != -1, "No root found on new body");
	HZ_ASSERT(result->GetRoot().m_ParentPart == nullptr, "Root is not a real root!");

	return result;
}

void Ship::CreatePhysicsBody(World& world, glm::vec2 pos, float rot)
{
	b2BodyDef def;
	def.position = b2Vec2(pos.x, pos.y);
	def.angle = rot;
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
