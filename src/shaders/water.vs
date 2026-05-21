#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 vUV;
out vec3 vWorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;
    vUV = aUV;
    gl_Position = projection * view * worldPos;
}
