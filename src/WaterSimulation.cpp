#include "WaterSimulation.h"

#include <algorithm>
#include <cmath>

WaterSimulation::WaterSimulation()
    : damping(N * N, 1.0f),
      normalTextureData(N * N * 4, 0),
      rng(std::random_device{}())
{
    z[0].assign(N * N, 0.0f);
    z[1].assign(N * N, 0.0f);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float l = std::min({i, j, N - 1 - i, N - 1 - j}) * h;
            damping[idx(i, j)] = 0.95f * std::min(1.0f, l / 0.2f);
        }
    }
}

void WaterSimulation::step() {

    // Obliczanie wysokości w nowym kroku
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 1; j < N - 1; ++j) {
            z[1 - curr][idx(i, j)] = 
                damping[idx(i, j)] * (
                A * (z[curr][idx(i + 1, j)] + z[curr][idx(i - 1, j)] + z[curr][idx(i, j - 1)] + z[curr][idx(i, j + 1)])
                + B * z[curr][idx(i, j)]
                - z[1 - curr][idx(i, j)]
            );
        }
    }

    // Warunki brzegowe
    for (int i = 0; i < N; ++i) {
        z[1 - curr][idx(i, 0)] = 0.0f;
        z[1 - curr][idx(i, N - 1)] = 0.0f;
    }

    curr = 1 - curr;

}

void WaterSimulation::maybeDropRain(float probability, float amplitude) {
    // W losowym punkcie siatki dodaje zaburzenie
    if (std::bernoulli_distribution(probability)(rng)) {
        int i = std::uniform_int_distribution<int>(4, N - 5)(rng);
        int j = std::uniform_int_distribution<int>(4, N - 5)(rng);
        disturb(i, j, amplitude);
    }
}

void WaterSimulation::disturb(int i, int j, float amplitude) {
    z[curr][idx(i, j)] += amplitude;
}

void WaterSimulation::uploadNormals(GLuint textureId) {
    const auto& h_curr = z[curr];
    const float inv2h = 1.0f / (2.0f * h);

    auto encode = [](float v) -> uint8_t {
        float clamped = std::clamp(v * 0.5f + 0.5f, 0.0f, 1.0f);
        return static_cast<uint8_t>(clamped * 255.0f + 0.5f);
    };

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            float nx, ny, nz;
            if (i == 0 || i == N - 1 || j == 0 || j == N - 1) {
                nx = 0.0f; ny = 1.0f; nz = 0.0f;
            } else {
                float dhdx = (h_curr[idx(i, j + 1)] - h_curr[idx(i, j - 1)]) * inv2h;
                float dhdz = (h_curr[idx(i + 1, j)] - h_curr[idx(i - 1, j)]) * inv2h;
                float vx = -dhdx;
                float vy = 1.0f;
                float vz = -dhdz;
                float invLen = 1.0f / std::sqrt(vx * vx + vy * vy + vz * vz);
                nx = vx * invLen;
                ny = vy * invLen;
                nz = vz * invLen;
            }
            int p = idx(i, j) * 4;
            normalTextureData[p + 0] = encode(nx);
            normalTextureData[p + 1] = encode(ny);
            normalTextureData[p + 2] = encode(nz);
            normalTextureData[p + 3] = 255;
        }
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, N, N,
                    GL_RGBA, GL_UNSIGNED_BYTE, normalTextureData.data());
}
