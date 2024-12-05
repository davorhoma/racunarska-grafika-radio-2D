#version 330 core

in vec2 chTex; //koordinate teksture
out vec4 outCol;

uniform sampler2D uTex; //teksturna jedinica

uniform float uTime;

void main()
{
	outCol = texture(uTex, vec2(chTex.s, chTex.t));
	if (outCol.a < 0.9)
	{
		discard;
	}
	//boja na koordinatama chTex teksture vezane na teksturnoj jedinici uTex
	//mijesanje 2 teksture se moze raditi sa mix(T1, T2, k) funkcijom gdje su
	//T1 i T2 pozivi texture funkcije a k koeficijent jacine druge teksture od 0 do 1 (0.3 = 70%T1 + 30%T2)
	//mijesanje sa bojom tjemena se moze odraditi mnozenjem vektora boja i tekstura
}