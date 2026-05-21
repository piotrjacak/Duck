#version 400 core

in vec3 vDir;

out vec4 FragColor;

uniform samplerCube uCubemap;

void main() {
    FragColor = texture(uCubemap, normalize(vDir));
}
