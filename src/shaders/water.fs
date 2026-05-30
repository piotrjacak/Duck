#version 400 core

in vec2 vUV;
in vec3 vWorldPos;

out vec4 FragColor;

uniform sampler2D   uNormalMap;
uniform samplerCube uCubemap;
uniform vec3        viewPos;
uniform vec3        cubeMin;
uniform vec3        cubeMax;
uniform float       eta;   // n_powietrza / n_wody
uniform float       F0;    // fresnel 

// Przeciecie promienia
vec3 sampleCubeAtBoxHit(vec3 origin, vec3 dir) {
    vec3 invDir = 1.0 / dir;
    vec3 t1 = (cubeMin - origin) * invDir;
    vec3 t2 = (cubeMax - origin) * invDir;
    vec3 tmax = max(t1, t2);
    float t = min(min(tmax.x, tmax.y), tmax.z);
    vec3 hit = origin + dir * t;
    return texture(uCubemap, normalize(hit)).rgb;
}

void main() {
    vec3 N = normalize(texture(uNormalMap, vUV).xyz * 2.0 - 1.0);
    vec3 V = normalize(vWorldPos - viewPos); 

    vec3 reflDir = reflect(V, N);
    vec3 refrDir = refract(V, N, eta);

    vec3 reflCol = sampleCubeAtBoxHit(vWorldPos, reflDir);
    vec3 refrCol = (length(refrDir) < 1e-4)
                   ? reflCol 
                   : sampleCubeAtBoxHit(vWorldPos, refrDir);

    // Niebieski kolor na refrakcji
    const vec3  waterColor   = vec3(0.1, 0.3, 0.6);
    const float waterDensity = 0.2;
    refrCol = mix(refrCol, waterColor, waterDensity);

    // Fresnel-Schlick
    float cosTheta = max(dot(-V, N), 0.0);
    float F = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);

    FragColor = vec4(mix(refrCol, reflCol, F), 1.0);
}
