#include "MouseController.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

MouseController::MouseController(Camera& camera) : camera(camera) {}

void MouseController::onMouseButton(int button, int action) {
    bool pressed = (action == GLFW_PRESS);
    if (button == GLFW_MOUSE_BUTTON_LEFT)  lmbDown = pressed;
    if (button == GLFW_MOUSE_BUTTON_RIGHT) rmbDown = pressed;

    if (action == GLFW_RELEASE && !lmbDown && !rmbDown) {
        firstMove = true;
    }
}

void MouseController::onCursorPos(double xpos, double ypos) {
    if (firstMove) {
        lastX = xpos;
        lastY = ypos;
        firstMove = false;
        return;
    }

    double dx = xpos - lastX;
    double dy = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    if (lmbDown) {
        camera.orbit(static_cast<float>(dx), static_cast<float>(-dy));
    }
    if (rmbDown) {
        camera.zoom(static_cast<float>(dy));
    }
}
