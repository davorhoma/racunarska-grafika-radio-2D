#version 330 core

out vec4 FragColor;

uniform vec3 dotColor;

void main() {
    float dist = length(gl_PointCoord - vec2(0.5, 0.5));

    if (dist > 0.5) {
        discard;
    }

    FragColor = vec4(dotColor, 1.0);
}

