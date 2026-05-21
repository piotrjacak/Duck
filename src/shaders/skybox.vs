#version 400 core

layout (location = 0) in vec3 aPos;

out vec3 vDir;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vDir = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
