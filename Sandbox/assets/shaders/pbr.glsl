#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextCoords;

out vec2 v_TextCoords;
out vec3 v_Normal;
out vec3 v_ToLightVector;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Model;

uniform vec3 u_LightPosition;

void main()
{
	v_TextCoords = a_TextCoords;
	v_Normal = a_Normal;
	gl_Position = u_ViewProjectionMatrix * u_Model * vec4(a_Position, 1.0f);
	v_ToLightVector = u_LightPosition - a_Position;
}


#type fragment
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
