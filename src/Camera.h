#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera(const glm::vec3& target = glm::vec3(0.0f),
           float yawDeg = -90.0f,
           float pitchDeg = 30.0f,
           float distance = 3.0f);

    glm::mat4 view() const;
    glm::mat4 projection(float aspect) const;

    void orbit(float dx, float dy);
    void zoom(float dy);

    glm::vec3 position() const;
    const glm::vec3& getTarget() const { return target; }

    void setOrbitSensitivity(float s) { orbitSens = s; }
    void setZoomSensitivity(float s)  { zoomSens = s; }

private:
    glm::vec3 target;
    float yaw;        // radians
    float pitch;      // radians
    float distance;

    float fovYDeg   = 60.0f;
    float nearPlane = 0.05f;
    float farPlane  = 100.0f;

    float orbitSens = 0.005f;
    float zoomSens  = 0.01f;

    float minDistance = 0.5f;
    float maxDistance = 50.0f;
};

#endif
