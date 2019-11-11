#pragma once
#include "hzpch.h"

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core.h"

namespace Hazel {

	struct Material
	{

		Material(Hazel::Ref<Texture2D>& albedo, float metallic, float roughness, float ao) : Albedo(albedo), Metallic(metallic), Roughness(roughness), AO(ao) {}

		Hazel::Ref<Texture2D> Albedo;
		float Metallic, Roughness, AO;
	};
}
