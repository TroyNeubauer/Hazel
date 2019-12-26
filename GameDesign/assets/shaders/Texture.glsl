// Basic Texture Shader

#type vertex

precision mediump float;

attribute vec3 a_Position;
attribute vec2 a_TexCoord;
attribute vec4 a_Color;

uniform mat4 u_ViewProjection;

varying vec2 v_TexCoord;
varying vec4 v_Color;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment

precision mediump float;

varying vec2 v_TexCoord;
varying vec4 v_Color;

uniform sampler2D u_Texture;

void main()
{
	vec4 textureColor = texture2D(u_Texture, vec2(v_TexCoord.x, 1.0 - v_TexCoord.y));
	if (textureColor.a < 0.001) discard;
	gl_FragColor = textureColor * v_Color;
}

