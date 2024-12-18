#version 330 core 

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTex;
out vec2 chTex;

uniform float uVibrationIntensity;

void main()
{
	float scale = 1.0 + 0.01 * sin(uVibrationIntensity);

    gl_Position = vec4(inPos * scale, 0.0, 1.0);
	chTex = inTex;
}