#version 330 core 

layout(location = 0) in vec2 inPos;

uniform float uScalePos;

void main()
{
    gl_Position = vec4(inPos.x + uScalePos, inPos.y, 0.0, 1.0);
//	chTex = inTex;
}