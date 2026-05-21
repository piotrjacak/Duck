#ifndef WATER_SIMULATION_H
#define WATER_SIMULATION_H

#include <glad/glad.h>

#include <vector>
#include <cstdint>
#include <random>


class WaterSimulation {
public:
    static constexpr int   N  = 256;
    static constexpr float h  = 2.0f / (N - 1);
    static constexpr float c  = 1.0f;
    static constexpr float dt = 1.0f / N;
    static constexpr float A  = c * c * dt * dt / (h * h);
    static constexpr float B  = 2.0f - 4.0f * A;

    WaterSimulation();

    // Jeden krok calkowania
    void step();

    // Dodaje zaburzenie w losowym miejscu
    void maybeDropRain(float probability = 0.02f, float amplitude = 0.25f);

    // Oblicza zaburzenie
    void disturb(int i, int j, float amplitude = 0.25f);

    // Liczy normalne do powierzchni
    void uploadNormals(GLuint textureId);

    // Dostep do bufora wysokości
    const std::vector<float>& heights() const { return z[curr]; }

private:
    // Dwa bufory wysokości: 0 - poprzedni, 1 - aktualny
    std::vector<float> z[2];
    std::vector<float> damping;        
    std::vector<uint8_t> normalTextureData; 

    int curr = 0;
    std::mt19937 rng;

    static int idx(int i, int j) { return i * N + j; }
};

#endif
