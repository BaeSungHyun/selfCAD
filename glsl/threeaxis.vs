#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColCoord;

out vec3 ColCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 camera;

void main() {
gl_Position = projection * camera * view * model * vec4(aPos, 1.0);
ColCoord = vec3(aColCoord.x, aColCoord.y, aColCoord.z);
}