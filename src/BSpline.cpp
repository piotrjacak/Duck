#include "BSpline.h"

#include <cmath>

BSpline::BSpline(int numControlPoints, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> dist(-0.8f, 0.8f);
    points.reserve(numControlPoints);
    for (int k = 0; k < numControlPoints; ++k) {
        points.emplace_back(dist(rng), 0.0f, dist(rng));
    }
}

namespace {

// pobiera 4 sasiednie pkt kontrolne
inline void unpack(float u,
                   const std::vector<glm::vec3>& pts,
                   float& t,
                   glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3)
{
    const int N = static_cast<int>(pts.size());
    int i = static_cast<int>(std::floor(u));
    t = u - i;
    i = ((i % N) + N) % N;

    p0 = pts[(i - 1 + N) % N];
    p1 = pts[i];
    p2 = pts[(i + 1) % N];
    p3 = pts[(i + 2) % N];
}

} // namespace

glm::vec3 BSpline::position(float u) const {
    float t;
    glm::vec3 p0, p1, p2, p3;
    unpack(u, points, t, p0, p1, p2, p3);

    const float t2 = t * t;
    const float t3 = t2 * t;

    // Bazowe funkcje B-Splina:
    const float b0 = (1.0f - 3.0f * t + 3.0f * t2 -        t3) / 6.0f;
    const float b1 = (4.0f             - 6.0f * t2 + 3.0f * t3) / 6.0f;
    const float b2 = (1.0f + 3.0f * t + 3.0f * t2 - 3.0f * t3) / 6.0f;
    const float b3 =                                         t3  / 6.0f;

    return b0 * p0 + b1 * p1 + b2 * p2 + b3 * p3;
}

glm::vec3 BSpline::tangent(float u) const {
    float t;
    glm::vec3 p0, p1, p2, p3;
    unpack(u, points, t, p0, p1, p2, p3);

    const float t2 = t * t;

    // Pochodne funkcji bazowych:
    const float d0 = -(1.0f - 2.0f * t + t2) * 0.5f;
    const float d1 = (9.0f * t2 - 12.0f * t) / 6.0f;
    const float d2 = (3.0f + 6.0f * t - 9.0f * t2) / 6.0f;
    const float d3 = t2 * 0.5f;

    return d0 * p0 + d1 * p1 + d2 * p2 + d3 * p3;
}
