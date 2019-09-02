#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TextCoords;
layout(location = 3) in float a_Brown;

out vec2 v_TextCoords;
out vec3 v_Normal;
out vec3 v_ToLightVector;
out float v_Brown;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Model;

uniform vec3 u_LightPosition;

void main()
{
	v_Brown = a_Brown;
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
	color = mix(albedo, vec4(122.0f/255.0f, 64.0f/255.0f, 2.0f/255.0f, 1.0f), v_Brown);
	color.a = 1.0f;
}
