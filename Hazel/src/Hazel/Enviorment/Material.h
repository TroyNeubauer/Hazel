#pragma once
#include "hzpch.h"

#include "Hazel/Renderer/Texture.h"
#include "Hazel/Core.h"

namespace Hazel {

	struct Material
	{
		Material() {}
		Material(Hazel::Ref<Texture2D>& albedo, float metalic, float roughness, float ao) : Albedo(albedo), Metalic(metalic), Roughness(roughness), AO(ao) {}

		Hazel::Ref<Texture2D> Albedo;
		float Metalic, Roughness, AO;
	};
}
