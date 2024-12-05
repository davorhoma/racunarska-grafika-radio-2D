#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
    float sampled = texture(text, TexCoords).r;
    FragColor = vec4(textColor, sampled);
    if (gl_FragCoord.x > 1023)
    {
        discard;
    }
}
