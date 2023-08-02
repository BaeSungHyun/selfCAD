#version 330 core

in vec3 ColCoord;

out vec4 FragColor;

void main() {
FragColor = vec4(ColCoord, 1.0);
}