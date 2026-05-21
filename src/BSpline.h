#ifndef BSPLINE_H
#define BSPLINE_H

#include <glm/glm.hpp>

#include <vector>
#include <cstdint>
#include <random>


class BSpline {
public:
    explicit BSpline(int numControlPoints = 8,
                     uint32_t seed = std::random_device{}());

    glm::vec3 position(float u) const;
    glm::vec3 tangent(float u) const;

    int segmentCount() const { return static_cast<int>(points.size()); }
    const std::vector<glm::vec3>& controlPoints() const { return points; }

private:
    std::vector<glm::vec3> points;
};

#endif
