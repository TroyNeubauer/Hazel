#include "Ships.h"


#include "Parts.h"

Hazel::Ref<EditorShip> Ships::MK1Ship = nullptr;

void Ships::Init()
{

	MK1Ship.reset(new EditorShip());

	Hazel::Ref<EditorPart> a = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(a);
	a->m_Def = Parts::MK1Capsule;
	a->m_ParentPart = nullptr;
	
	Hazel::Ref<EditorPart> b = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(b);
	b->m_Def = Parts::MK1Engine;
	b->m_Offset = { 0.0f, -a->m_Def->HitboxSize.y / 2.0f - b->m_Def->HitboxSize.y / 2.0f };
	b->m_ParentPart = a;

}
