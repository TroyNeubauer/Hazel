#pragma once
#include "hzpch.h"

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core.h"

namespace Hazel {

	struct Material
	{
		Material(): Metallic(0.0f), Roughness(0.0f), AO(0.0f) {}

		Material(Hazel::Ref<Texture2D>& albedo, float metallic, float roughness, float ao) : Albedo(albedo), Metallic(metallic), Roughness(roughness), AO(ao) {}

		Hazel::Ref<Texture2D> Albedo;
		float Metallic, Roughness, AO;
	};
}
