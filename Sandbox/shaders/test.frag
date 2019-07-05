#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TextCoords;
in vec3 v_Normal;
in vec3 v_ToLightVector;
in float v_Brown;

uniform sampler2D sampler;

uniform vec3 u_LightColor;

void main()
{
	vec4 albedo = texture(sampler, v_TextCoords);
	if(albedo.a < 0.5f)
		discard;
	float difuseLighting = dot(v_Normal, v_ToLightVector);
	difuseLighting = max(difuseLighting, 0.0f);//No negitive lighting

	color = vec4(difuseLighting * u_LightColor, 1.0f) * albedo;
	color = mix(albedo, vec4(122f/255f, 64f/255f, 2f/255f, 1.0f), v_Brown);
	color.a = 1.0f;
}
