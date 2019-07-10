#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TextCoords;
in vec3 v_Normal;
in vec3 v_ToLightVector;

uniform sampler2D sampler;

void main()
{
	vec4 albedo = texture(sampler, v_TextCoords);
	if(albedo.a < 0.5f)
		discard;

	color = albedo;
	color.a = 1.0f;
}
