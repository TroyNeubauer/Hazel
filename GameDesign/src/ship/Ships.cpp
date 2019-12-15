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

	Hazel::Ref<EditorPart> c = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(c);
	c->m_Def = Parts::MK1LeftWing;
	c->m_Offset = { -a->m_Def->HitboxSize.x / 2.0f - c->m_Def->HitboxSize.x / 2.0f, -5.0f / 3 };
	c->m_ParentPart = a;

	Hazel::Ref<EditorPart> d = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(d);
	d->m_Def = Parts::MK1RightWing;
	d->m_Offset = { a->m_Def->HitboxSize.x / 2.0f + d->m_Def->HitboxSize.x / 2.0f, -5.0f / 3 };
	d->m_ParentPart = a;

}
