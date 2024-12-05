#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; //teksturna jedinica

uniform int maxPos;

void main()
{
	outCol = texture(uTex, vec2(chTex.s, chTex.t));
	if ((outCol.a < 0.1) || (gl_FragCoord.x >= maxPos + 30))
	{
		discard;
	}
}