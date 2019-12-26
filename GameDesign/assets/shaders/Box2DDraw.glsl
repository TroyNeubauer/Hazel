// Basic Texture Shader

#type vertex
precision mediump float;

attribute vec2 a_Position;
attribute vec4 a_Color;

uniform mat4 u_ViewProjection;

varying vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
}

#type fragment

precision mediump float;

varying vec4 v_Color;

void main()
{
	gl_FragColor = vec4(v_Color.rgb, 1.0);
}

