#version 330 core

in vec2 chTex;
out vec4 outCol;

uniform sampler2D uTex;

void main()
{
	outCol = texture(uTex, vec2(chTex.s, chTex.t));
	if (outCol.a < 0.1 || gl_FragCoord.y < 680)
	{
		discard;
	}
}