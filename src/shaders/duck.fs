#version 400 core

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3      lightPos;
uniform vec3      viewPos;
uniform vec3      lightColor;

void main() {
    vec3 baseColor = texture(uTexture, vUV).rgb;

    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightPos - vWorldPos);
    vec3 V = normalize(viewPos  - vWorldPos);
    vec3 R = reflect(-L, N);

    vec3 ambient  = 0.15 * baseColor;
    vec3 diffuse  = max(dot(N, L), 0.0) * lightColor * baseColor;
    vec3 specular = pow(max(dot(V, R), 0.0), 32.0) * lightColor * 0.3;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
