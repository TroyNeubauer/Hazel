#include "Ships.h"


#include "Parts.h"

Hazel::Ref<EditorShip> Ships::MK1Ship = nullptr;

void Ships::Init()
{

	MK1Ship.reset(new EditorShip());

	Hazel::Ref<EditorPart> a = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(a);
	a->Def = Parts::MK1Capsule;
	a->ParentPart = nullptr;
	
	Hazel::Ref<EditorPart> b = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(b);
	b->Def = Parts::MK1Engine;
	b->m_Offset = { 0.0f, -a->Def->HitboxSize.y / 2.0f - b->Def->HitboxSize.y / 2.0f };
	b->ParentPart = a;

	Hazel::Ref<EditorPart> c = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(c);
	c->Def = Parts::MK1LeftWing;
	c->m_Offset = { -a->Def->HitboxSize.x / 2.0f - c->Def->HitboxSize.x / 2.0f, -5.0f / 3 };
	c->ParentPart = a;

	Hazel::Ref<EditorPart> d = Hazel::R(new EditorPart());
	MK1Ship->GetParts().push_back(d);
	d->Def = Parts::MK1RightWing;
	d->m_Offset = { a->Def->HitboxSize.x / 2.0f + d->Def->HitboxSize.x / 2.0f, -5.0f / 3 };
	d->ParentPart = a;

}
