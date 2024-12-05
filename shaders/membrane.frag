#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform sampler2D uMembraneTex;
uniform sampler2D uCoverTex;

uniform float uTime;

void main()
{
	outCol = texture(uMembraneTex, vec2(chTex.s, chTex.t));
	if (outCol.a < 0.4)
	{
		discard;
	}
}