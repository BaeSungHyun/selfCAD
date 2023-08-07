#versino 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 Col;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 camera;

void main() { 
gl_Position = projection * camera * model * vec4(aPos, 1.0);
Col = vec3(aCol.x, aCol.y, aCol.z);
}