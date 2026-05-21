#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

namespace {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float PITCH_LIMIT = 89.0f * PI / 180.0f;
}

Camera::Camera(const glm::vec3& target, float yawDeg, float pitchDeg, float distance)
    : target(target),
      yaw(yawDeg * PI / 180.0f),
      pitch(pitchDeg * PI / 180.0f),
      distance(distance) {}

glm::vec3 Camera::position() const {
    float cp = std::cos(pitch);
    float sp = std::sin(pitch);
    float cy = std::cos(yaw);
    float sy = std::sin(yaw);
    return target + distance * glm::vec3(cp * cy, sp, cp * sy);
}

glm::mat4 Camera::view() const {
    return glm::lookAt(position(), target, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::projection(float aspect) const {
    if (aspect <= 0.0f) aspect = 1.0f;
    return glm::perspective(glm::radians(fovYDeg), aspect, nearPlane, farPlane);
}

void Camera::orbit(float dx, float dy) {
    yaw   += dx * orbitSens;
    pitch -= dy * orbitSens;
    pitch = std::clamp(pitch, -PITCH_LIMIT, PITCH_LIMIT);
}

void Camera::zoom(float dy) {
    distance *= std::exp(dy * zoomSens);
    distance = std::clamp(distance, minDistance, maxDistance);
}
