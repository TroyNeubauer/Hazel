#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextCoords;

out vec2 v_TextCoords;
out vec3 v_Normal;
out vec3 v_WorldPos;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Model;

void main()
{
	v_TextCoords = a_TextCoords;
	v_Normal = a_Normal;

	vec4 clipWorldPos = u_Model * vec4(a_Position, 1.0f);
	v_WorldPos = clipWorldPos.xyz;
	gl_Position = u_ViewProjectionMatrix * clipWorldPos;
}


#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TextCoords;
in vec3 v_Normal;
in vec3 v_WorldPos;

uniform sampler2D u_Albedo;
uniform sampler2D u_Metallic;
uniform sampler2D u_Roughness;
uniform sampler2D u_NormalMap;
uniform sampler2D u_AOMap;
uniform sampler2D u_HeightMap;

uniform vec3 u_CamPos;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;

uniform float u_AO;

const float PI = 3.14159265359;


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;
	
	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float num   = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	
	return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 V = normalize(u_CamPos - v_WorldPos);
	vec3 N = v_Normal;

	vec3 albedo = texture(u_Albedo, v_TextCoords).rgb;
	float roughness = texture(u_Roughness, v_TextCoords).r;
	float metallic = texture(u_Metallic, v_TextCoords).r;

	vec3 mappedNormal = texture(u_NormalMap, v_TextCoords).rgb;
	float ao = texture(u_AOMap, v_TextCoords).r;
	float mappedHeight = texture(u_HeightMap, v_TextCoords).r;

	// reflectance equation
	vec3 F0 = albedo;
	vec3 Lo = vec3(0.0f);
	
	vec3 L = normalize(u_LightPosition - v_WorldPos);
	vec3 H = normalize(V + L);
	float distance = length(u_LightPosition - v_WorldPos);
	float attenuation = 1.0f / (distance * distance);
	vec3 radiance = u_LightColor * attenuation;

	// cook-torrance brdf
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	// add to outgoing radiance Lo
	float NdotL = max(dot(N, L), 0.0);
	Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;
	
	//Gamma correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));
	
	FragColor = vec4(color, 1.0);
	//FragColor = vec4(color, 1.0) * 0.00001f + vec4(mappedNormal, 1.0f);
}
