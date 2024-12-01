#version 330 core

out vec4 FragColor;

uniform vec3 dotColor;

void main() {
    // Calculate distance from the center of the point
    float dist = length(gl_PointCoord - vec2(0.5, 0.5));

    // If the distance is greater than 0.5 (radius of the circle), discard the fragment
    if (dist > 0.5) {
        discard;
    }

    // Set the color of the circle
    FragColor = vec4(dotColor, 1.0);
}

